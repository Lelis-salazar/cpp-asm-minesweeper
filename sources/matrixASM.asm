; https://stackoverflow.com/questions/8504097/declaring-variable-sized-arrays-in-assembly
;mov rax, 100   # 100 elements
;shl rax, 3     # multiply with 8, the size of an element
;sub rsp, rax   # rsp now points to your array

;# do something with the array
;mov rbx, [rsp]    # load array[0] to rbx
;mov [rsp+8], rbx  # store to array[1]

;add rsp, rax   # rsp points to the return address again

;https://stackoverflow.com/questions/20510132/what-exactly-does-malloc-do-in-assembly
;mov edi, 40; malloc's first (and only) parameter: number of bytes to allocate
;extern malloc
;call malloc
; on return, rax points to our newly-allocated memory
;mov ecx,7; set up a constant
;mov [rax],ecx; write it into memory
;mov edx,[rax]; read it back from memory
;mov eax,edx; copy into return value register
;ret

section .data
  MINE equ -1
  ; Llamado del sistema para pedir memoria
  SYS_mmap equ 9
  ; Llamado del sistema para liberar memoria
  SYS_munmap equ 11
  SYS_exit equ 60

section .text

global matrix_constructor
matrix_constructor:
  ; rdi = this
  ; [rdi] = rowsQty
  ; [rdi + 4] = colsQty
  ; [rdi + 8] = board*
  ; [rdi + 16] = boardBytes
  ; esi = int rowsQty
  ; edx = int colsQty
  ; ecx = int mineQty
  ; almacena en la pila los registros modificados
  push rbp
  push rbx
  mov rbp, rsp
  ; almacena la direccion de this a rbx
  mov rbx, rdi 
  ; rowsQty atributo
  mov dword [rbx], esi
  ; colsQty atributo
  mov dword [rbx + 4], edx
  ; multiplica rows * cols para determinar el total de celdas 
  mov eax, dword [rbx]
  mul dword [rbx + 4]
  ; guarda la cantidad de celdas para pedir memoria
  mov rsi, rax
  ; guarda la cantidad de celdas para el loop
  mov r12, rax
  ; rdi << 2 = rdi*4 (obtiene el total de bytes, 2 bytes por celda por ser int)
  shl rsi, 2
  ; almacena cantidad de bytes en boardBytes
  mov dword[rbx + 16], esi
  ; Argumentos para pedir memoria
  ; void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
  ; addr = NULL
  ; length = boardBytes rsi
  ; prot = PROT_READ | PROT_WRITE (1 | 2 = 3)
  ; flags = MAP_PRIVATE | MAP_ANONYMOUS (2 | 32 = 34)
  ; fd = 0 (not used)
  ; offset = 0 (not used)
  mov     rax, SYS_mmap   ; llamado del sistema para pedir memoria
  xor     rdi, rdi        ; addr = NULL
  ;rsi = boardBytes
  mov     rdx, 3          ; prot = PROT_READ | PROT_WRITE
  mov     r10, 34         ; flags = MAP_PRIVATE | MAP_ANONYMOUS
  xor     r8, r8          ; fd = 0
  xor     r9, r9          ; offset = 0
  syscall
  cmp rax, 0
  JE error
  mov rcx, r12
  ; board* attribute, almacena la direccion de memoria obtenida
  mov [rbx + 8], rax
  mov r10, 0
  ; inicializa todas las celdas en 0
  initializeMatrix:
    mov dword[rax + r10], 0
    add r10, 4
    loop initializeMatrix
    mov rax, 0
    jmp terminar
  error:
    mov rax, 1
  terminar:
    pop rbx
    pop rbp
  ret

global matrix_destructor
matrix_destructor:
  ; rdi = this
  ; [rdi+8] = board*
  ; [rdi + 16] = boardBytes
  push rbp
  push rbx
  mov rbp, rsp
  ; Argumentos para liberar memoria
  ; int munmap(void *addr, size_t length);
  ; addr = address to unmap
  ; length = boardBytes
  ;  almacena la direccion de this a rbx
  mov rbx, rdi
  ; almacena la direccion addr a liberar
  mov rax, [rbx + 8]
  ; compara que no este ya liberada
  cmp rax, 0
  je skipFree
  ; asigna la direccion a liberar en rdi
  mov rdi, rax
  ; llamado al sistema para liberar memoria
  mov rax, SYS_munmap
  mov rsi, qword [rbx+16]  
  syscall
  skipFree:
    ; asigna nullptr a board*
    mov qword [rbx + 8], 0
    pop rbx
    pop rbp
  ret

global checkCellAsm
checkCellAsm:
  push rbp
  push r15
  mov rbp, rsp
  ; rdi = this
  ; esi = int x
  ; edx = int y
  ; ecx = int newValue
  ; convierte las coordenadas a lineales
  call Convert2DtoLinearPos
  ; mueve board* a r15
  mov r15, [rdi + 8]
  ; selecciona el valor de la celda y la guarda en el reg de retorno
  mov eax, [r15 + rax*4]
  pop r15
  pop rbp
  ret
global updateCellAsm
updateCellAsm:
  push rbp
  push r15
  mov rbp, rsp
  ; rdi = this
  ; esi = int x
  ; edx = int y
  ; ecx = int newValue
  ; convierte las coordenadas a lineales
  call Convert2DtoLinearPos
  ; mueve board* a r15
  mov r15, [rdi + 8]
  ; selecciona el valor de la celda y la guarda en el reg de retorno
  mov dword [r15 + rax*4], ecx
  pop r15
  pop rbp
  ret
global placeMineAsm
placeMineAsm:
  ; rdi = this
  ; esi = int x
  ; edx = int y
  push rbp
  push r15
  mov rbp, rsp
  ; convierte las coordenadas a lineales
  call Convert2DtoLinearPos
  mov r15, [rdi + 8]
  ; rdi + 8 = board*, eax*4 = (x,y) traducido a lineal. *4 por ser int
  cmp dword [r15 + rax*4], MINE
  je isMine
  ; asigna el valor de la mina a la celda
  mov dword [r15 + rax*4], MINE
  call increaseCounterAsm
  mov eax, 1
  jmp isNotMine
  isMine:
    ; int minePlaced = 0, if 0 mine was not placed if 1 mine was placed
    mov eax, 0
  isNotMine:
  pop r15
  pop rbp
  ret
; @brief incrementa las celdas aledañas en 1 ya que estan funcionan como pistas
; para identificar donde se encuentran las minas
;
; @param self Puntero This del objeto. 
; @param x coordenada x de la celda (fila).
; @param y coordenada y de la celda (columna)
global increaseCounterAsm
increaseCounterAsm:
  push rbp
  push rbx
  mov rbp, rsp
  ; rdi = this
  ; esi = int x
  ; edx = int y
  ; convierte (x, y) a vector lineal
  call Convert2DtoLinearPos
  cmp esi, 0
  je case_x_0
  ; rowsQty attribute
  mov ebx, [rdi]
  ; rowsQty - 1
  dec ebx
  cmp ebx, esi
  je case_x_rows_minus_1
  jmp case_x_default
  ;----------------------------------------------------------------------------
  ; case x = 0
  ;----------------------------------------------------------------------------
  case_x_0:
    cmp edx, 0
    je case_x_0_y_0
    ; colsQty attribute
    mov ebx, [rdi+4]
    dec ebx
    cmp ebx, edx
    je case_x_0_y_cols_minus_1
    jmp case_x_0_y_default
    ;--------------------------------------------------------------------------
    ; case x = 0, y = 0
    ;--------------------------------------------------------------------------
    case_x_0_y_0:
    ;(x, y + 1)++
    call IncXYplus1
    ; x + 1
    inc esi
    ; convierte (x+1, y) a vector lineal
    call Convert2DtoLinearPos
    ;(x+1, y)++
    call IncXY
    ;(x+1, y+1)++
    call IncXYplus1
    jmp end
    ;--------------------------------------------------------------------------
    ; case x = 0, y = cols - 1
    ;--------------------------------------------------------------------------
    case_x_0_y_cols_minus_1:
    ; (x, y-1)++
    call IncXYminus1
    ; x + 1
    inc esi
    ; convierte (x+1, y) a vector lineal
    call Convert2DtoLinearPos
    ; (x+1, y)++
    call IncXY
    ; (x+1, y-1)++
    call IncXYminus1
    jmp end
    ;--------------------------------------------------------------------------
    ; case x = 0, y = default 
    ;--------------------------------------------------------------------------
    case_x_0_y_default:
    ; (x, y-1)++
    call IncXYminus1
    ; (x, y+1)++
    call IncXYplus1
    ; x + 1
    inc esi
    ; convierte (x+1, y) a vector lineal
    call Convert2DtoLinearPos
    ; (x+1, y-1)++
    call IncXYminus1
		; (x+1,y)++
    call IncXY
		; (x+1, y+1)++
    call IncXYplus1
    jmp end
  ;----------------------------------------------------------------------------
  ; case x = rows - 1 
  ;----------------------------------------------------------------------------
  case_x_rows_minus_1:
    cmp edx, 0
    je case_x_rows_minus_1_y_0
    ; colsQty attribute
    mov ebx, [rdi+4]
    dec ebx
    cmp ebx, edx
    je case_x_rows_minus_1_y_cols_minus_1
    jmp case_x_rows_minus_1_y_default
    ;--------------------------------------------------------------------------
    ; case x = rows - 1, y = 0
    ;--------------------------------------------------------------------------
    case_x_rows_minus_1_y_0:
    ; (x, y+1)++
    call IncXYplus1
    ; x - 1
    dec esi
    ; convierte (x-1, y) a vector lineal
    call Convert2DtoLinearPos
    ; (x-1, y)++
    call IncXY
		; (x-1, y+1)++
    call IncXYplus1
    jmp end
    ;--------------------------------------------------------------------------
    ; case x = rows - 1, y = cols - 1
    ;--------------------------------------------------------------------------
    case_x_rows_minus_1_y_cols_minus_1:
    ; (x, y-1)++
    call IncXYminus1
    ; x - 1
    dec esi
    ; convierte (x-1, y) a vector lineal
    call Convert2DtoLinearPos
    ; (x-1, y)++
    call IncXY
    ; (x-1, y-1)++
    call IncXYminus1
    jmp end
    ;--------------------------------------------------------------------------
    ; case x = rows - 1, y = default
    ;--------------------------------------------------------------------------
    case_x_rows_minus_1_y_default:
    ;(x, y+1)++
    call IncXYplus1
    ;(x, y-1)++	
    call IncXYminus1
    ; x - 1
    dec esi
    ; convierte (x-1, y) a vector lineal
    call Convert2DtoLinearPos
    ;(x-1, y)++
    call IncXY
    ;(x-1, y+1)++
    call IncXYplus1
    ;(x-1, y-1)++
    call IncXYminus1
    jmp end
  ;----------------------------------------------------------------------------
  ; case x = default
  ;----------------------------------------------------------------------------
  case_x_default:
    cmp edx, 0
    je case_x_default_y_0
    ; colsQty attribute
    mov ebx, [rdi+4]
    dec ebx
    cmp ebx, edx
    je case_x_default_y_cols_minus_1
    jmp case_x_default_y_default
    ;--------------------------------------------------------------------------
    ; case x = default, y = 0
    ;--------------------------------------------------------------------------
    case_x_default_y_0:
    ; (x, y+1)++
    call IncXYplus1
    ; x - 1
    dec esi
    ; convierte (x-1, y) a vector lineal
    call Convert2DtoLinearPos
    ; (x-1, y)++
    call IncXY
		; (x-1, y+1)++
    call IncXYplus1
    ; x + 1
    add esi, 2
    ; convierte (x+1, y) a vector lineal
    call Convert2DtoLinearPos
		; (x+1, y)++
    call IncXY
		; (x+1, y+1)++
    call IncXYplus1
    jmp end
    ;--------------------------------------------------------------------------
    ; case x = default, y = cols - 1
    ;--------------------------------------------------------------------------
    case_x_default_y_cols_minus_1:
    ; (x, y-1)++
    call IncXYminus1
    ; x - 1
    dec esi
    ; convierte (x-1, y) a vector lineal
    call Convert2DtoLinearPos
    ; (x-1, y)++
    call IncXY
    ; (x-1, y-1)++
    call IncXYminus1
    ; x + 1
    add esi, 2
    ; convierte (x+1, y) a vector lineal
    call Convert2DtoLinearPos
		; (x+1, y)++
    call IncXY
		; (x+1, y-1)++
    call IncXYminus1
		jmp end
    ;--------------------------------------------------------------------------
    ; case x = default, y = default
    ;--------------------------------------------------------------------------
    case_x_default_y_default:
    ; (x, y-1)++
    call IncXYminus1
		; (x, y+1)++
    call IncXYplus1
    ; x - 1
    dec esi
    ; convierte (x-1, y) a vector lineal
    call Convert2DtoLinearPos
    ; (x-1, y)++
    call IncXY
    ; (x-1, y-1)++
    call IncXYminus1
		; (x-1, y+1)++
    call IncXYplus1
    ; x + 1
    add esi, 2
    ; convierte (x+1, y) a vector lineal
    call Convert2DtoLinearPos
    ; (x+1, y)++
    call IncXY
		; (x+1, y-1)++
    call IncXYminus1
		; (x+1, y+1)++	
    call IncXYplus1
  end:
  ;mov rsp, rbp
  pop rbx
  pop rbp
  ret
; @brief Convierte las coordenadas (x,y) a coordenadas lineales
;
; @param self Puntero This del objeto. 
; @param x coordenada x de la celda (fila).
; @param y coordenada y de la celda (columna)
; @return coordenada convertida a lineal
global Convert2DtoLinearPos
Convert2DtoLinearPos:
  push rbp
  push rbx
  mov rbp, rsp
  ; rdi = this
  ; esi = int x
  ; edx = int y 
  ; mueve la cantidad de columnas a eax
  mov eax, [rdi + 4]
  ; mueve int y a rbx porque ebx es usado en la multiplicacion
  mov ebx, edx
  ; multiplica la cantidad x por la cantidad de columnas
  mul esi
  ; suma la posicion 'y' con x*columnas
  add eax, ebx
  ; returns int y attribute to its original value
  mov edx, ebx
  ;mov rsp, rbp
  pop rbx
  pop rbp
  ret
; @brief Incrementa las celdas en 1 para X, Y+1
;
; @param self Puntero This del objeto. 
; @param x coordenada x de la celda (fila).
; @param y coordenada y de la celda (columna)
global IncXYplus1
IncXYplus1:
  push rbp
  push r15
  mov rbp, rsp
  ; rdi = this
  ; rdi + 8 = board[]
  ; rax*4 = (x,y) traducido a lineal *4 por ser int 
  ; rdi + 8 + rax*4 + 4 = x || x+1 || x -1, y + 1
  ; is (x || x + 1 || x - 1, y+1) a mine? 
  mov r15, [rdi +8]
  cmp dword [r15 + rax*4 + 4], MINE
  je end_IncXYplus1
  ; (x || x + 1 || x - 1, y+1)++
  inc dword [r15 + rax*4 + 4]
  end_IncXYplus1:
  ;mov rsp, rbp
  pop r15
  pop rbp
  ret
; @brief Incrementa las celdas en 1 para X, Y
;
; @param self Puntero This del objeto. 
; @param x coordenada x de la celda (fila).
; @param y coordenada y de la celda (columna)
global IncXY
IncXY:
  push rbp
  push r15
  mov rbp, rsp
  ; rdi = this
  ; rdi + 8 = board[]
  ; rax*4 = (x,y) traducido a lineal *4 por ser int 
  ; rdi + 8 + rax*4 = x || x+1 || x -1, y 
  ; is (x || x + 1 || x - 1, y) a mine? 
  mov r15, [rdi +8]
  cmp dword [r15 + rax*4], MINE
  je end_IncXY
  ; (x || x + 1 || x - 1, y)++
  inc dword [r15 + rax*4]
  end_IncXY:
  ;mov rsp, rbp
  pop r15
  pop rbp
  ret
; @brief Incrementa las celdas en 1 para X, Y -1
;
; @param self Puntero This del objeto. 
; @param x coordenada x de la celda (fila).
; @param y coordenada y de la celda (columna)
global IncXYminus1
IncXYminus1:
  push rbp
  push r15
  mov rbp, rsp
  ; rdi = this
  ; rdi + 8 = board[]
  ; rax*4 = (x,y) traducido a lineal *4 por ser int 
  ; rdi + 8 + rax*4 - 4 = x || x+1 || x -1, y - 1
  ; is (x || x + 1 || x - 1, y-1) a mine? 
  mov r15, [rdi +8]
  cmp dword [r15 + rax*4 - 4], MINE
  je end_IncXYminus1
  ; (x || x + 1 || x - 1, y-1)++
  inc dword [r15 + rax*4 - 4]
  end_IncXYminus1:
  ;mov rsp, rbp
  pop r15
  pop rbp
  ret