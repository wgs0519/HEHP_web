let port;
let reader;
let writer;

const log = (message) => {
    const logElement = document.getElementById('log');
    logElement.textContent += message + '\n';
};

document.getElementById('connect').addEventListener('click', async () => {
    try {
        port = await navigator.serial.requestPort();
        await port.open({ baudRate: 115200 });
        log('디바이스 연결됨');
        reader = port.readable.getReader();
        writer = port.writable.getWriter();
        document.getElementById('updateFirmware').disabled = false;
    } catch (error) {
        log('디바이스 연결 실패: ' + error);
    }
});

document.getElementById('updateFirmware').addEventListener('click', async () => {
    try {
        const response = await fetch('https://api.github.com/repos/USERNAME/REPO/releases/latest');
        const data = await response.json();
        const firmwareUrl = data.assets[0].browser_download_url;
        const firmwareResponse = await fetch(firmwareUrl);
        const firmwareArrayBuffer = await firmwareResponse.arrayBuffer();
        const firmware = new Uint8Array(firmwareArrayBuffer);

        log('펌웨어 다운로드 완료, 플래싱 시작');
        
        await enterFlashingMode();
        await flashFirmware(firmware);
        await exitFlashingMode();

        log('펌웨어 업로드 완료');
    } catch (error) {
        log('펌웨어 업로드 실패: ' + error);
    } finally {
        reader.releaseLock();
        writer.releaseLock();
        await port.close();
        log('디바이스 연결 해제됨');
    }
});

const enterFlashingMode = async () => {
    log('플래싱 모드로 전환 중...');
    // 플래싱 모드로 진입하기 위해 필요한 시퀀스를 여기에 작성합니다.
    // 예: GPIO0 핀을 LOW로 설정 후 리셋
};

const flashFirmware = async (firmware) => {
    log('펌웨어 전송 중...');
    const chunkSize = 0x1000; // 일반적으로 4096 바이트 단위로 전송합니다.
    for (let offset = 0; offset < firmware.length; offset += chunkSize) {
        const chunk = firmware.slice(offset, offset + chunkSize);
        await sendChunk(chunk, offset);
        log(`플래싱 중... (${offset + chunk.length}/${firmware.length})`);
    }
};

const sendChunk = async (chunk, offset) => {
    // ESP32 플래싱 프로토콜에 맞게 청크를 전송합니다.
    // 여기에 CRC 계산 및 기타 필요한 작업을 추가합니다.
    const command = createFlashCommand(chunk, offset);
    const response = await sendCommand(command, 10); // 응답 길이는 상황에 따라 조정
    if (!verifyResponse(response)) {
        throw new Error('청크 전송 실패');
    }
};

const exitFlashingMode = async () => {
    log('플래싱 모드 종료 중...');
    // 플래싱 모드를 종료하고 ESP32를 재부팅합니다.
};

const createFlashCommand = (chunk, offset) => {
    // 플래싱 명령을 생성합니다.
    // 여기에서 CRC 계산과 명령 패킷을 생성하는 로직을 추가합니다.
    return new Uint8Array([...]);
};

// Helper function to send data and read response
const sendCommand = async (command, responseLength) => {
    await writer.write(command);
    const response = new Uint8Array(responseLength);
    let received = 0;
    while (received < responseLength) {
        const { value, done } = await reader.read();
        if (done) {
            break;
        }
        response.set(value, received);
        received += value.length;
    }
    return response;
};

const verifyResponse = (response) => {
    // 응답을 검증합니다.
    // 이 부분은 ESP32 플래싱 프로토콜에 따라 달라질 수 있습니다.
    return true;
};
