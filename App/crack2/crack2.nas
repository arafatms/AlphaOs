[INSTRSET "i486p"]
[BITS 32]
		MOV		EAX,1*8			; OS�p�̃Z�O�����g�ԍ�
		MOV		DS,AX			; �����DS�ɂ��ꂿ�Ⴄ
		MOV		BYTE [0x102600],0
		MOV		EDX,4
		INT		0x40
