; This file handles all communication with the N64 Controller
; It is written to make optimal use of CPU cycles in order to capture all data
; timing based on 48MHz clock, nop's used to delay
#include <P18F14K50.INC>

; TODO: format this properly - directives shouldn't be in colummn 1,
; just labels and comments

cblock
    countLow    ; signal pulse low count
    countHigh   ; signal pulse high count

    output_byte ; the byte we're outputting
    bit_count ; number of bits we're reading/writing
endc

CODE

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Macros
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Macro for delaying for a given number of cycles (pre-set in delay_reg)
DELAY_LOOP  macro delay_reg
            local delay
delay:
            decfsz delay_reg
            bra delay
            endm

; Waits while the input line is either high or low
WAIT_LINE   macro timeout, high
            local loop
            movlw timeout
loop:
            dcfsnz WREG
            bra discardData     ; if we count down to 0, abort
            if high == 1;
                btfsc PORTC, 2      ; continue if low (loop if high)
            else
                btfss PORTC, 2      ; continue if high (loop if low)
            endif
            bra loop
            endm

; Macro to delay while input line is low - wreg is set to timeout
; minus the number of cycles we were in the loop (useful for timing)
WAIT_LINE_LOW   macro timeout
                WAIT_LINE timeout, 0
                endm

; Macro to delay while input line is high - wreg is set to timeout
WAIT_LINE_HIGH  macro timeout
                WAIT_LINE timeout, 1
                endm

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Exported functions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; sends the desired command to the controller, MSB first
N64CommSendCommand
    bsf TRISC, 2 ; make sure we're in high impedence mode for now
    bcf LATC, 2 ; This will allow us to output 0
    
    ; Read the parameters from the stack
    ; BYTE cmd - byte we're sending
    movlw 0xff
    movff PLUSW1, output_byte

    ; We're outputting 8 bits
    movlw 0x08
    movwf bit_count

SendBit:
    ; Get the next bit to send out
    movlw 0x09 ; start out for a one (3us high)
    btfss output_byte, 7
        movlw 0x01 ; gets us to ~11 cycles before setting trisc, 5 would get us to 14... maybe a nop?

        ;;;;;; low counts
        ; 4 gets us to ~11 cycles before setting trisc, 5 would get us to 14... maybe a nop?
        ; 12 gets us to ~35 cycles before setting trisc, 13 would get us to 38

        ;;;;; High counts
        ; 1 gets us to ~11 cycles, 2 gets us to 14
        ; 9 gets us to ~35, 10 gets us to 38
    movwf countHigh
    sublw 0x0D    ; figure out how long we need to be low
    ;
    ; now output with the timings set before

    ; Above here is ~5 cycles

    ; Output low
    bcf TRISC, 2

    ; delay for number of cycles in wreg
    DELAY_LOOP WREG

    ; Output high
    bsf TRISC, 2

    ; delay for number of cycles in countHigh
    DELAY_LOOP countHigh

    ; below is ~ 4 cycles that we need to account for

    ; Shift left and loop if we haven't finished outputting yet
    rlcf output_byte
    decfsz bit_count
        bra SendBit

    ; TODO: send stop bit
    bcf TRISC, 2
    movlw 0x04 ; hold low for 1 us (12 cycles)
    DELAY_LOOP WREG
    bsf TRISC, 2

    ; We can return now - since we're outputting one, we have at least 3us
    ; before the controller should be sending a response. Since there's a pullup
    ; on the line, we don't need to do anything more with the output
return
    
;read the response
N64CommReadData
    ; Read in the params 
    ; void *data - the pointer to the output buffer
    movlw 0xff
    movff PLUSW1, FSR0H
    decf WREG
    movff PLUSW1, FSR0L
    
    ; BYTE size - the size of the data we're reading
    decf WREG
    movff PLUSW1, bit_count
    
	; Wait for the line to go low (delay while high)
    WAIT_LINE_HIGH 0x7F

ReadBit:
    ; count amount of time line is low (timeout isn't full 0x7F because we ate a few cycles into the next bit)
	WAIT_LINE_LOW 0x7E

	movwf countLow ; Copy the low count out for later comparison

	; count amount of time line is high
	WAIT_LINE_HIGH 0x7F

    ; determine which part was longer (high vs low)
    ; store FF or 00 to represent 1 or 0
    subwf countLow, W ; TODO: idea - just increment the register at the start, then decrement it for the "waitLow" - if it's 0 at this point, the second half was longer...saves instructions
    clrf INDF0
    btfsc STATUS, C
        setf INDF0
    incf FSR0L          ; Go to next adress in the array

    ; reset count variables for next loop
    movlw 0x7E ; not the full 0x7F because we ate a few cycles into the next bit

    ; TODO: (been a while) but it looks like this ignores the stop bit - should look into this and make proper note of it
    decfsz bit_count    ; end loop if all expected bits have been read
        bra ReadBit

    ;indicate no data error (return 0)
    movlw 0x00
    MOVWF PRODL
return

discardData
    movlw 0x01
    MOVWF PRODL  ; Indicate there was an error (error code 1)
return

; export functions to C
GLOBAL N64CommSendCommand
Global N64CommReadData

end
