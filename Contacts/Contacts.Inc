include windows.inc
include msvcrt.inc		;����C���ԵĿ�
includelib msvcrt.lib	;����C���Կ��Ӧ��lib�ļ�

include     kernel32.inc
includelib  kernel32.lib

.data
;����ṹ�塢����ȫ�ֱ���
CONTACTSSTRUCT struct
	szName	BYTE 25 dup(0)		;����
	szPhNumber BYTE 12 dup(0)	;�绰����
CONTACTSSTRUCT ends
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
PCONTACTSSTRUCT TYPEDEF PTR CONTACTSSTRUCT			;ȡ������ָ�����ͣ�
;[�µ�������]	TYPEDEF PTR [ԭ����������]			;��ʾȡһ��ָ�����͵�����
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


;����ȫ�ֱ���
g_stContacts	CONTACTSSTRUCT 100 dup(<'0','0'>)	;����ṹ������
g_nCount		DWORD		   0				;Ԫ�ظ���
g_nCountMax		DWORD		   100				;�����Ԫ��
g_strTemContacts	CONTACTSSTRUCT <'0','0'>	;����������Ϣ
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


;�����ʽ���Ʒ�	�����������ʱ�õ��ĸ�ʽ���Ʒ�
g_szScanfFormat		BYTE	'%s %s',0
g_szScanName		BYTE	'%s',0
g_nChoose		dword	100					;�˵�ѡ��
g_numScanfFormat		byte	'%d',0		;�˵�ѡ���ʽ�������

;�ļ�����
g_szFileWB	BYTE	'wb',0
g_szFileRB	byte	'rb',0
g_pFile	dword 0
g_szFile	byte	'data.text',0

;system����
g_szCls		byte 'cls',0				;����
g_szPause	byte	'pause',0			;��ͣ
g_szZero	byte	' ',0dh,0ah,0		;���ڻ���

;��ʾ
g_szAddStr byte	'�����룺�û���	�绰����',0dh,0ah,0
g_szFindInfo    byte  	"����������:",0dh,0ah,0
g_szInputError	byte	'�������',0dh,0ah,0
g_szTipNewName	byte	'���������û��� �绰����',0dh,0ah,0
g_szTipNoExist	byte	'�û�������...',0dh,0ah,0
g_szTipNoDate	byte	'û������...',0dh,0ah,0
g_szTipListOver	byte	'��ѯ���...',0dh,0ah,0
g_szOK			byte	'�����ɹ���',0
g_szMenu		byte	'�����ѡ���������',0dh,0ah,
						'1 - �鿴�û�',0dh,0ah,
						'2 - �����û�',0dh,0ah,
						'3 - �����û�',0dh,0ah,
						'4 - �޸��û�',0dh,0ah,
						'5 - ɾ���û�',0dh,0ah,
						'0 - �˳�',0dh,0ah,0
						
g_szTipSizeError	byte	'�������ݳ�������',0dh,0ah,0
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>



.code
;�����û���Ϣ
ADD_USER proc		;�޲���
	
		push eax
		push ebx				;�������õ��üĴ������ȱ���һ��ԭ���Ĵ�����ֵ
		lea eax, g_szAddStr			;��ʾ�ڶ����������ĵ�ַ�������һ����������
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;1.ͨ��push���������Σ�4�ֽڣ�
;2.���ú���	���ǵ��õ���C���ԵĿ⺯��������Ҫ����ͷ�ļ���
;	include	msvcrt.inc
;	includelib	msvcrt.lib
;	�⺯���ĸ�ʽΪ��crt_xxx
;3.C�⺯�������ɵ�����ƽ���ջ
;	ÿ�������Ĵ�СΪ4�ֽڣ����Դ��뼸����������֮��esp�ӻ���
;	add esp,[��������*4]
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	
	push eax	;push r/m32(imu32)	�����ǼĴ������ڴ��������
	call crt_printf
	add esp,4
	
	;����ecx��ֵ�ҵ���һ���ṹ����������ĵ�ַ
	
	
	lea esi,[g_stContacts]		;�������ݵĽṹ������
	mov ecx,g_nCount		;��ȡ��ǰ�Ѳ�����û�����
	mov eax,sizeof(CONTACTSSTRUCT)	;����ṹ��Ĵ�С��sizeof�꣩
	imul eax,ecx
	add esi,eax			;�ƶ��ṹ�������ָ�루�û�����*�ṹ��Ĵ�С��
	
	;����crt_scanf�����������������
	lea eax,[esi+CONTACTSSTRUCT.szPhNumber]		;��һ���������绰���룩
	lea edx,[esi+CONTACTSSTRUCT.szName]		;��һ��������������
	push eax
	push edx
	push offset g_szScanfFormat			;��ʽ���Ʒ�
	call crt_scanf
	add esp,0ch					;ƽ���ջ
	inc g_nCount					;�û�������1
	
	;��������
	call SaveDate
	
	
	;����ɹ�
	push offset g_szOK				;�����ɹ�
	call crt_printf
	add esp,4
	

	;��ͣ
	push offset g_szPause
	call crt_system
	add esp,4
	
	
	pop eax						;������ʼpushָ���Ӧ
	pop ebx	
		
	ret

ADD_USER endp



;��ѯ�û���Ϣ
FindData proc
;1.��������

	;���������Ϣ
	push offset g_szFindInfo
	call crt_printf
	add esp,4
	
	;��մ����ʱ���ݵĽṹ��
	lea edi,[g_strTemContacts]	;����ṹ��ĵ�ַ
	mov ebx,sizeof(CONTACTSSTRUCT)
	push ebx
	push 0h
	push edi
	call crt_memset
	add esp,12
	
	lea edi,[g_strTemContacts.szName]	;����ṹ�������ֵĵ�ַ
	push edi
	push offset g_szScanName		;��ʽ���Ʒ�
	call crt_scanf	
	add esp,8				;ƽ���ջ
	
;2.��ʼ��ѯ
	mov ecx,0				;��ʼ��ѭ��������Ĭ�ϴ�0��ʼѭ����
CYCLE_MARK:
	cmp ecx,g_nCount			;�ж��Ƿ����ѭ��
	jz END_F
	
;2.1����ecx��ֵ�ҵ���һ���ṹ����������ĵ�ַ
	
	mov eax,sizeof(CONTACTSSTRUCT)		;����ṹ��Ĵ�С��sizeif�꣩
	imul eax,ecx
	lea esi,[g_stContacts]			;�������ݵĽṹ������
	add esi,eax
	
;2.2�Ƚ��ַ���
	mov eax,ecx				;�������ѭ���Ĵ���
	mov ecx,6				;��ʼ����������ѭ��������4�ֽڱȽϣ�
	
	
	
	
	
	lea edi,[g_strTemContacts.szName]	;���浱ǰ������û����ֵĵ�ַ
	repe cmpsd dword ptr[esi],dword ptr[edi]	;�鿴repeϵ��ָ���ʹ��ԭ��
	je CARRIEDOUT_MARK				;����ҵ�����ת�������Ϣ��
	mov ecx,eax				;���û���ҵ���������ѭ��
	inc ecx 				;��ѭ��������1
	jmp CYCLE_MARK				;��������ת�����ѭ����ʼλ��
	
CARRIEDOUT_MARK:
	;�����Ϣ
	mov ecx,eax
	lea esi,[g_stContacts]
	mov ebx,sizeof(CONTACTSSTRUCT)
	imul ebx,ecx
	add esi,ebx
	lea eax,[esi+CONTACTSSTRUCT.szPhNumber]
	push eax
	push offset g_szScanName
	call crt_printf
	add esp,8
	
	;����
	push offset g_szZero
	call crt_printf
	add esp,4
	
	push offset g_szOK								;�����ɹ�
	call crt_printf
	add esp,4

	
	;��ͣ
	push offset g_szPause 
	call crt_system
	add esp,4
	
	ret
END_F:
	push offset g_szTipNoExist
	call crt_printf
	add esp,4
	;��ͣ
	push offset g_szPause
	call crt_system
	add esp,4
	ret
FindData endp



;�޸��û���Ϣ
ModifyData proc
	
	;��Ϊ�޸���Ϣ�ĵ�һ��Ҳ����Ҫ����ǰ�������Ϣ���ѱ���������в�ѯ
;1.��������

	;���������Ϣ
	push offset g_szFindInfo
	;push offset	g_szScanName
	call crt_printf
	add esp,4
	
		;��մ����ʱ���ݵĽṹ��
	lea edi,[g_strTemContacts]	;����ṹ��ĵ�ַ
	mov ebx,sizeof(CONTACTSSTRUCT)
	push ebx
	push 0h
	push edi
	call crt_memset
	add esp,12

	lea edi,[g_strTemContacts.szName]	;����ṹ�������ֵĵ�ַ
	push edi
	push offset g_szScanName			;��ʽ���Ʒ�
	call crt_scanf
	add esp,8							;ƽ���ջ
;2.��ʼ��ѯ
	mov ecx,0							;��ʼ��ѭ��������Ĭ�ϴ�0��ʼ��	
CYCLE_MARK:								;���
	cmp ecx,g_nCount					;�ж��Ƿ����ѭ��
	jz END_M
	
	;2.1����ecx��ֵ�ҵ���һ���ṹ����������ĵ�ַ
	lea esi,[g_stContacts]				;�������ݵĽṹ������
	lea edi,[g_strTemContacts.szName]	;��ȡ��ǰ����Ҫ��ѯ���û����ֵ�ַ
	mov eax,sizeof(CONTACTSSTRUCT)		;����ṹ��Ĵ�С��sizeof�꣩
	imul eax,ecx
	add esi,eax
	
	;2.2�Ƚ��ַ���
	mov eax,ecx							;�������ѭ���Ĵ���
	mov ecx,6							;��ʼ������������ѭ��������4�ֽڱȽϣ�
	repe cmpsd dword ptr[esi],dword ptr[edi]		
	je CARRIEDOUT_MARK					;����ҵ�����ת��������Ϣ��
	mov ecx,eax							;���û���ҵ���������ѭ��
	inc ecx								;��ѭ��������1
	jmp CYCLE_MARK						;��������ת�����ѭ����ʼλ��
	
	
CARRIEDOUT_MARK:
	;�޸���Ϣ
	mov ecx,eax
	lea esi,[g_stContacts]
	mov ebx,sizeof(CONTACTSSTRUCT)
	imul ebx,ecx
	add esi,ebx
	
	;��������Ϣ
	push offset g_szTipNewName
	call crt_printf
	add esp,4
	
	
	lea ebx,[esi+CONTACTSSTRUCT.szName]
	lea eax,[esi+CONTACTSSTRUCT.szPhNumber]
	push eax
	push ebx
	push offset g_szScanfFormat
	call crt_scanf
	add esp,0Ch
	
;��������
	call SaveDate
	
	push offset g_szOK								;�����ɹ�
	call crt_printf
	add esp,4

	
	;��ͣ
	push offset g_szPause
	call crt_system
	add esp,4
	
	ret
END_M:
	push offset g_szTipNoExist
	call crt_printf
	add esp,4
	;��ͣ
	push offset g_szPause
	call crt_system
	add esp,4
	ret
ModifyData endp



;ɾ���û���Ϣ	
RemoveDate proc
;1.��������

	;���������Ϣ
	push offset g_szFindInfo
	;push offset	g_szScanName
	call crt_printf
	add esp,4
	
		;��մ����ʱ���ݵĽṹ��
	lea edi,[g_strTemContacts]	;����ṹ��ĵ�ַ
	mov ebx,sizeof(CONTACTSSTRUCT)
	push ebx
	push 0h
	push edi
	call crt_memset
	add esp,12

	lea edi,[g_strTemContacts.szName]				;����ṹ�������ֵĵ�ַ
	push edi
	push offset g_szScanName						;��ʽ���Ʒ�
	call crt_scanf
	add esp,8										;ƽ���ջ
	
;2.��ʼ��ѯ
	mov ecx,0										;��ʼ��ѭ��������Ĭ�ϴ�0��ʼѭ����
CYCLE_MARK:
	cmp ecx,g_nCount								;�ж��Ƿ����ѭ��
	jz END_M
	
	;2.1����ecx��ֵ�ҵ���һ���ṹ����������ĵ�ַ
	lea esi,[g_stContacts]							;�������ݵĽṹ������
	lea edi,[g_strTemContacts.szName]				;��ȡ��ǰ�����Ҫ��ѯ�û����ֵ�ַ
	mov eax,sizeof(CONTACTSSTRUCT)					;����ṹ��Ĵ�С
	imul eax,ecx
	add esi,eax
	
	;2.2�Ƚ��ַ���
	mov eax,ecx										;�������ѭ���Ĵ���
	mov ecx,6										;��ʼ����������ѭ��������4�ֽڱȽϣ�
	repe cmpsd dword ptr[esi],dword ptr[edi]			
	je CARRIEDOUT_MARK
	mov ecx,eax										;û���ҵ���������ѭ��
	inc ecx											;��ѭ��������1
	jmp CYCLE_MARK									;��������ת�����ѭ����ʼλ��
	
CARRIEDOUT_MARK:
	;ɾ��
	;��esi����Ϊ��ǰҪɾ���Ľṹ��������׵�ַ		
	mov ecx,eax										;eax���������ȡ���ı�ʾ��ǰ�ҵ��������λ��
	lea edi,[g_stContacts]
	mov ebx,sizeof(CONTACTSSTRUCT)
	imul ebx,ecx
	add edi,ebx										;edi��ʱ������ǵ�ǰҪɾ���Ľṹ��������׵�ַ
	mov esi,edi										
	mov ebx,sizeof(CONTACTSSTRUCT)
	add esi,ebx										;esiָ��Ҫɾ���Ľṹ���������һ��Ԫ�ص��׵�ַ
	
	add ecx,1										;��ΪҪ��������ʱ�Ǵ�����0��ʼ�ģ����Լ�1���ڼ���
													;��Ҫ�ƶ����ٸ�Ԫ�أ��м�ĳһ��Ԫ�ر�ɾ���ˣ�����
													;��Ԫ����ǰ�ƶ�
	mov eax,g_nCount		
	sub eax,ecx										;��Ҫ�ƶ��Ĵ���
	mov ebx,sizeof(CONTACTSSTRUCT)
	imul ebx,eax									;������Ҫ�ƶ����ֽ�
	mov ecx,ebx											
	rep movs BYTE ptr[edi],BYTE ptr[esi]			;��ʼ�ƶ�����һ���ֽڵĴ�С�ƶ���
	
	;�ƶ���ɺ�ɾ�����һ���ṹ���е���Ϣ
	mov ebx,sizeof(CONTACTSSTRUCT)
	push ebx										;��С
	push 0											;����
	push edi										;ɾ�����׵�ַ
	call crt_memset									;���ó�ʼ������
	add esp,12
	dec g_nCount
	

	;��������
	call SaveDate	

	push offset g_szOK								;�����ɹ�
	call crt_printf
	add esp,4

	;��ͣ
	push offset g_szPause
	call crt_system
	add esp,4
	
	ret
END_M:
	push offset g_szTipNoExist
	call crt_printf
	add esp,4
	;��ͣ
	push offset g_szPause
	call crt_system
	add esp,4
	ret

RemoveDate endp



;�˵�
Menu proc
	;��ӡ�˵�
	push offset g_szMenu
	call crt_printf
	add esp,4				;ƽ���ջ
	
	ret
	
Menu endp



;�˵�ѡ���֧��ת
MenuJump proc
	;�Ƚ�������������������1-������2
	cmp g_nChoose,0			
	jz OP0			
	
	cmp g_nChoose,1	
	jz OP1
		
	cmp g_nChoose,2
	jz OP2
		
	cmp g_nChoose,3	
	jz OP3
		
	cmp g_nChoose,4	
	jz OP4
	
	cmp g_nChoose,5
	jz OP5
	
	;�������
	push offset g_szInputError
	call crt_printf
	add esp,4
	


	
	;��ͣ
	push offset g_szPause
	call crt_system
	add esp,4
	
	;INVOKE GetStdHandle, STD_INPUT_HANDLE ;��ȡ������,STD_INPUT_HANDLE��win32����
	;mov consoleInHandle,eax ;����
    ;INVOKE FlushConsoleInputBuffer,consoleInHandle ;�������(ͨ���Ǽ���)���庯ʽ

	
	jmp EndMenu_Mark
	
	;�˳�	
OP0:
	;exitProcess(0)
	push    0
    call    ExitProcess
    add esp,4
	jmp EndMenu_Mark

	
	;��ѯ
OP1:
	call ListAll
	jmp EndMenu_Mark
	
	;����
OP2:
	call ADD_USER
	jmp EndMenu_Mark
	
	;����
OP3:
	call FindData
	jmp EndMenu_Mark

	;�޸�
OP4:
	call ModifyData 
	jmp EndMenu_Mark


	;ɾ��
OP5:
	call RemoveDate
	jmp EndMenu_Mark

 
EndMenu_Mark:	
	ret

MenuJump endp



;�鿴�����û�
ListAll proc
	xor ecx,ecx
;1.�Աȵ�ǰ����
	cmp g_nCount,0
	je NODATE
	jne CYCLE_lIST
	
;������
NODATE:
	push offset g_szTipNoDate
	call crt_printf
	add esp,4
	jmp EndList_Mark
	
;�г�����
	
CYCLE_lIST:
	push ecx
	
	;2.1��ַ
	lea esi,[g_stContacts]
	mov ebx,sizeof(CONTACTSSTRUCT)
	imul ebx,ecx
	add esi,ebx										;��ecx���ṹ���ַ
	
	
	;printf("%s%s",a,b)
	lea eax,[esi+CONTACTSSTRUCT.szPhNumber]
	push eax
	lea eax,[esi+CONTACTSSTRUCT.szName]			
	push eax
	push offset g_szScanfFormat
	call crt_printf
	add esp,0ch
	
	push offset g_szZero
	call crt_printf
	add esp,4

	pop ecx
	inc ecx												;��һ����ϵ��ecx��0�����ԱȽ�ǰ��Ҫ��1
    cmp dword ptr [g_nCount],ecx					;�Ƚϵ�ǰ���λ��ecx   ͨѶ¼����g_nCount
    jle EndList_Mark									;��ȴ���ȫ����ӡ�꣬������ѯ����
    
    ;������ϵ��û�д�ӡ
    ;inc ecx			;ǰ���Ѿ���1�� �����ﲻ��Ҫ�ټ�	
    jmp CYCLE_lIST
    
;��ѯ����
EndList_Mark:

	;��ѯ���
	push offset g_szTipListOver
	call crt_printf
	add esp,4
	
    ;��ͣ
	push offset g_szPause
	call crt_system
	add esp,4
	
    ret
ListAll endp



;����
SaveDate proc
	
	push ebp
	mov ebp,esp
	
	;�����ļ�ָ��
	mov g_pFile,0
	
	;���ļ�
	push offset g_szFileWB
	push offset g_szFile
	call crt_fopen
	add esp ,04h
	mov dword ptr [g_pFile],eax
	
;size_t fwrite(const void* buffer, size_t size, size_t count, FILE* stream);  
   ;-- buffer:ָ�����ݿ��ָ��  
   ;-- size:ÿ�����ݵĴ�С����λΪByte(���磺sizeof(int)����4)  
   ;-- count:���ݸ���  
   ;-- stream:�ļ�ָ��
   
;�洢��ϵ������
	push g_pFile		;�ļ�ָ��
	push 1				;���ݸ���
	push 4				;ÿ�����ݵĴ�С ������dword 4�ֽ�
	lea ebx,[g_nCount]	;����ָ��
	push ebx
	call crt_fwrite
    add esp,10h 

	
;�洢�ļ��ṹ������
	;�ļ�ָ��
	push g_pFile		;�ļ�ָ�� 		
	
	;���ݸ���
	lea eax,g_nCount
	push eax
	
    ;���ݴ�С
    mov ebx,sizeof(CONTACTSSTRUCT)
    push ebx
    
    ;�ṹ�������׵�ַ
    lea esi,[g_stContacts]
    push esi

    call crt_fwrite
    add esp,10h 


    ;�ر��ļ�
    push g_pFile  
    call crt_fclose
    add esp,4
    mov dword ptr [g_pFile],0

    mov esp,ebp
    pop ebp
	
	ret
SaveDate endp


LoadDate proc
	push ebp
	mov ebp,esp
	
	;�����ļ�ָ��
	mov g_pFile,0
	
	;���ļ�
	push offset g_szFileRB
	push offset g_szFile
	call crt_fopen
	add esp,08h
	
	mov g_pFile,eax				;���ļ�����ֵeax��eaxΪ0��ʧ�ܣ��ɹ�Ϊ1
	cmp g_pFile,0
	je End_Load					;��ʧ�ܣ���ת
	

	;size_t fread(void *buffer, size_t size, size_t count, FILE *stream);  
  		;-- buffer:ָ�����ݿ��ָ��  
  		;-- size:ÿ�����ݵĴ�С����λΪByte(���磺sizeof(int)����4)  
  		;-- count:���ݸ���  
  		;-- stream:�ļ�ָ��  
	
	;��ȡ��ϵ������
	push g_pFile				;�ļ�ָ��
	push 1
	push 4
	lea esi,[g_nCount]			;���ݵ�ַ
	push esi
	call crt_fread
	add esp,10h
	
	
	;��ȡ��ϵ�˽ṹ����������
	push g_pFile
	mov eax,g_nCount
	push eax
	mov ebx,sizeof(CONTACTSSTRUCT)
	push ebx
	lea esi,[g_stContacts]
	push esi
	call crt_fread
	add esp,10h
	
End_Load:
	;�ر��ļ�
	push g_pFile
	call crt_fclose
	add esp,4
	
	mov g_pFile,0
	
	mov esp,ebp
	pop ebp
	
	ret

LoadDate endp


	
	
	
	
	
	
	
	
	
	
	
	
	
	
	


