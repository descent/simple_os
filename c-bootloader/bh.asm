	org	07c00h			; 告訴編譯器程序加載到7c00處
	mov	ax, cs
	mov	ds, ax
	mov	es, ax
	call	DispStr			; 使用顯示字元串例程
	jmp	$			; 無限循環
DispStr:
	mov	ax, BootMessage
	mov	bp, ax			; ES:BP = 串位址
	mov	cx, 16			; CX = 串長度
	mov	ax, 01301h		; AH = 13,  AL = 01h
	mov	bx, 000ch		; 頁號為0(BH = 0) 黑底紅字(BL = 0Ch,高亮)
	mov	dl, 0
	int	10h			; 10h 號中斷
	ret
BootMessage:		db	"Hello, NASM!"
times 	510-($-$$)	db	0	; 填充剩下的空間，使生成的二進制程式碼恰好為512字節
dw 	0xaa55				; 結束標誌
