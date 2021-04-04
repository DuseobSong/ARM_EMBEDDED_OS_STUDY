uint8_t Hal_uart_get_char(void){
	uint8_t data;
	wile(Uart->uarfr.bits.RXFE);

	// Check for an error flag
	if(Uart->uartdr.bits.BE || Uart->uartdr.bits.FE ||
			Uart->uartdr.bits.OE || Uart->uartdr.bits.PE){
		// Clear the error
		Uart->uartrsr.bits.BE = 1;
		Uart->uartrsr.bits.FE = 1;
		Uart->uartrsr.bits.OE = 1;
		Uart->uartrsr.bits.PE = 1;
		return 0;
	}

	data = Uart->uartdr.bits.DATA;
	return data;
}


/*
 * 각 에러 플래그 4개를 개별적으로 확인 -> 레지스트리에 대한 접근이 네 번 발생하고 각각의 비트 플래그를 확인하므로 비트 시프트 연산 및 0이 아닌지 확인하는 연산이 각각 네 번씩 발생한다. 
 *
 * arm-linux-gnueabihf-objdump -d Uart.o 명령어를 사용하면 340바이트짜리 바이너리가 생성된다. 
 *
 */

// 1차 최적화
uint8_t Hal_uart_get_char(void){
	uint8_t data;
	while(Uart->uartfr.bits.RXFE);

	// Check for an error flag
	if(Uart->uartdr.all & 0xFFFFFF00){
		// clear the error
		Uart->uarrsr.all = 0xFF;
		return 0;
	}

	data = Uart->uartdr.bits.DATA;
	return data;
}

/*
 *32비트 레지스터 자체로 접근하여 32비트 값을 직접 비교하고 입력하는 코드로 변경레지스터 접근은 두 번으로 줄고 비교 및 입력 연산은 각각 한 번으로 감소
 바이너리의 크기는 148 바이트로 감소

 최종적으로 최적화된 코드는 hal/rvpb/Uart.c 참조
 */

