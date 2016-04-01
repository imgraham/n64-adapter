; This file handles all communication with the N64 Controller
; It is written to make optimal use of CPU cycles in order to capture all data
; timing based on 48MHz clock, nop's used to delay
#include <P18F14K50.INC>

; TODO: format this properly - directives shouldn't be in colummn 1,
; just labels and comments
; TODO: pass these in via the stack instead of globals
EXTERN controller_data

cblock
    d4  ; bit bount
    countLow    ; signal pulse low count
    countHigh   ; signal pulse high count

    output_byte ; the byte we're outputting
    bit_count ; number of bits we're outputting
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

; set up bits to send for controller ID request
IdentifyController			; Send the controller ID signal (0b00000000)
    ; Part 1 - Set up the number of bits to be sent and read
    movlw 0x18              ; 0x18 = 24 bits
    movwf d4                ; contains controller identifier

    movlw 0x00  ; 0x01 is the "Identify" command
    movwf output_byte
    bra sendCmd

; set up bits to send for controller poll request
PollController              ; Send the polling signal (0b00000001)

    ; Part 1 - Read the N64 controller and store the status values
    movlw 0x20              ; 0x20 = 32 bits
    movwf d4                ; contains the number of buttons

    movlw 0x01  ; 0x01 is the "Poll" command
    movwf output_byte

    ;movlw 0x08
    movlw 0x09 ; TODO: temporary for lack of proper stop bit
    movwf bit_count
    bra sendCmd

; sends the desired command to the controller, MSB first
sendCmd
    bsf TRISC, 2 ; make sure we're in high impedence mode for now
    bcf LATC, 2 ; This will allow us to output 0

    LFSR 0, controller_data	; set up the array address for storing response

SendBits:
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
        bra SendBits

    ; TODO: send stop bit

;read the response
	;countLow and countHigh set before polling starts

	; Wait for the line to go low (delay while high)
    WAIT_LINE_HIGH 0x7F

readLoop:
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
    decfsz d4           ; end loop if all 32 bits are read
        bra readLoop

    ;indicate no data error (return 0)
    movlw 0x00
    MOVWF PRODL
return

discardData
    movlw 0x01
    MOVWF PRODL  ; Indicate there was an error (error code 1)
return

; export functions to C
GLOBAL PollController
Global IdentifyController

end
