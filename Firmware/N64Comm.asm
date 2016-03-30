; This file handles all communication with the N64 Controller
; It is written to make optimal use of CPU cycles in order to capture all data
#include <P18F14K50.INC>

EXTERN controller_data
EXTERN controller_data_error

cblock
    d1
    d2
    d3
    d4
	countLow
	countHigh
endc

CODE

PollController              ; Send the polling signal (0b000000011)

	; Part 1 - Read the N64 controller and store the status values
    movlw 0x20              ; 0x20 = 32 bits
    movwf d4                ; contains the number of buttons
             
	LFSR 0,controller_data	; set up the array address

    movlw 0x07              ; number of zeros that need to be sent
    movwf d2

    movlw 0x01              ; number of ones that need to be sent
    movwf d3

	movlw 0x7F
	movwf countLow
	movwf countHigh

    zero
		bcf LATC, 2
    	bcf TRISC, 2

        movlw 0x0B
        movwf d1
        zeroloop            ; 9 instruction cycles
            decfsz d1
            goto zeroloop

    	bsf TRISC, 2
		nop
		nop
		nop
		nop
		nop
		nop
		nop
        decfsz d2
        goto zero

    one
		bcf LATC, 2
    	bcf TRISC, 2

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

    	bsf TRISC, 2
		nop
        movlw 0x07
        movwf d1
        oneloop             ; 8 instruction cycles
            nop
            decfsz d1
            goto oneloop
        decfsz d3
        goto one

	stopBit
		bcf LATC, 2
    	bcf TRISC, 2

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

    	bsf TRISC, 2
		nop
        movlw 0x02
        movwf d1
        sbloop             ; 8 instruction cycles
            nop
            decfsz d1
            goto sbloop

	;read the response
    bsf TRISC, 2            ; RB7 as input  (is also last instruction of poll)	

	;;;;;
	;countLow and countHigh set before polling starts

	;wait for line to go low
	waitLowLoop
		dcfsnz countLow
			goto discardData
		btfsc PORTC, 2      ; continue if low
		goto waitLowLoop

	movlw 0x7F
	movwf countLow

    readLoop                ; read all 33 bits and store them on locations 0x20 etc.

		waitHigh
			dcfsnz countLow
				goto discardData
			btfss PORTC, 2      ; continue if low
			goto waitHigh

		waitLow
			dcfsnz countHigh
				goto discardData
			btfsc PORTC, 2      ; continue if low
			goto waitLow

		nextBit

		; determine which part was longer (high vs low)
		; store FF or 00 to represent 1 or 0
		movf countLow, W
		subwf countHigh, W
		movlw 0xFF
		btfsc STATUS, C
		movlw 0x00
        movwf INDF0         ; Wait & store the value of the bit in the array
        incf FSR0L          ; Go to next adress in the array

		; reset count variables for next loop
		movlw 0x7F
        movwf countLow
		movwf countHigh

        decfsz d4           ; end loop if all 33 bits are read
    goto readLoop

	;indicate no data error
	LFSR 0,controller_data_error
	movlw 0x00
    movwf INDF0         ; Wait & store the value of the bit in the array

return

discardData
	LFSR 0,controller_data_error
	movlw 0xFF
    movwf INDF0         ; Wait & store the value of the bit in the array
return

; export PollController function to C
GLOBAL PollController

end