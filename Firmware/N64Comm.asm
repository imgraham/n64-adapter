; This file handles all communication with the N64 Controller
; It is written to make optimal use of CPU cycles in order to capture all data
; timing based on 48MHz clock, nop's used to delay
#include <P18F14K50.INC>

; TODO: format this properly - directives shouldn't be in colummn 1, 
; just labels and comments
; TODO: pass these in via the stack instead of globals
EXTERN controller_data
EXTERN controller_data_error

cblock
    d1  ; generic counter for delay loops
    d2  ; zero count
    d3  ; one count
    d4  ; bit bount
    countLow    ; signal pulse low count
    countHigh   ; signal pulse high count
endc

CODE

; set up bits to send for controller ID request
IdentifyController			; Send the controller ID signal (0b00000000)
    ; Part 1 - Set up the number of bits to be sent and read
    movlw 0x18              ; 0x18 = 24 bits
    movwf d4                ; contains controller identifier

    movlw 0x08              ; number of zeros that need to be sent
    movwf d2

    movlw 0x00              ; number of ones that need to be sent
    movwf d3
    goto sendCmd

; set up bits to send for controller poll request
PollController              ; Send the polling signal (0b00000001)

    ; Part 1 - Read the N64 controller and store the status values
    movlw 0x20              ; 0x20 = 32 bits
    movwf d4                ; contains the number of buttons

    movlw 0x07              ; number of zeros that need to be sent
    movwf d2

    movlw 0x01              ; number of ones that need to be sent
    movwf d3
    goto sendCmd

; sends the desired command to the controller
sendCmd
    bsf TRISC, 2

    LFSR 0, controller_data	; set up the array address for storing response
    
    movlw 0x7F
    movwf countLow
    movwf countHigh

    ; line is toggled on/off by manipulating tristate (pull up facilitates this)
    ; send zeros to controller
    zero
	bcf LATC, 2
    	bcf TRISC, 2

        ; high delay (3 us)
        movlw 0x0B
        movwf d1
        zeroloop            ; 9 instruction cycles
            decfsz d1
            goto zeroloop

    	bsf TRISC, 2

    ; low delay (1 us)
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	decfsz d2
	goto zero

	;skip of no 1's to send
	movf	d3

    ; send ones to controller
    one
	bcf LATC, 2
    	bcf TRISC, 2

	btfss STATUS, Z
	    goto stopBit ; done sending ones

        ; high delay (1 us)
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop		

        ; low delay (3 us)
    	bsf TRISC, 2
	    nop
        movlw 0x07
        movwf d1
        oneloop             ; 8 instruction cycles
            nop
            decfsz d1
		goto oneloop
	    decf d3
	    goto one

    stopBit
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop		

	; this isn't the full high time since we want to start waiting for the controller response
        ; (there is no difference between reading and outputting 1)
    	bsf TRISC, 2
	nop
	nop
	nop

	;read the response
	;countLow and countHigh set before polling starts

	;wait for line to go low
	waitLowLoop
	    dcfsnz countLow
		goto discardData
	    btfsc PORTC, 2      ; continue if low
		goto waitLowLoop

	movlw 0x7D	;don't set right at 7F since this takes a few cycles

    readLoop            ; read all 33 bits and store them on locations 0x20 etc.
        ; count amount of time line is high
	waitHigh
	    dcfsnz WREG
		goto discardData
	    btfss PORTC, 2      ; continue if low
		goto waitHigh

	movwf countLow ; Copy the low count out for later comparison
	movlw 0x7F

	; count amount of time line is low
	waitLow
	    dcfsnz WREG
		goto discardData
	    btfsc PORTC, 2      ; continue if low
		goto waitLow

	nextBit
	    ; determine which part was longer (high vs low)
	    ; store FF or 00 to represent 1 or 0
	    subwf countLow, W ; TODO: idea - just increment the register at the start, then decrement it for the "waitLow" - if it's 0 at this point, the second half was longer...saves instructions
	    movlw 0x00
	    btfsc STATUS, C
            movlw 0xFF
	    movwf INDF0         ; Wait & store the value of the bit in the array
	    incf FSR0L          ; Go to next adress in the array

	    ; reset count variables for next loop
	    movlw 0x7D ; not the full 0x7F because we ate a few cycles into the next bit

	    decfsz d4           ; end loop if all 33 bits are read
		goto readLoop

    ;indicate no data error
    movlw 0x00
    movwf controller_data_error

return

discardData
    movlw 0xFF
    movwf controller_data_error  ; Indicate there was an error
return

; export functions to C
GLOBAL PollController
Global IdentifyController

end