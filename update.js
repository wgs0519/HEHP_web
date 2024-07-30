let port;
let logElement = document.getElementById('log');

const log = (message) => {
    logElement.textContent += message + '\n';
};

document.getElementById('connect').addEventListener('click', async () => {
    try {
        log('디바이스 연결 버튼 클릭됨');
        port = await navigator.serial.requestPort();
        await port.open({ baudRate: 115200 });
        log('시리얼 포트 열림');
        document.getElementById('updateFirmware').disabled = false;
        log('업데이트 버튼 활성화');
    } catch (error) {
        log('디바이스 연결 실패: ' + error.message);
        console.error('디바이스 연결 실패:', error);
    }
});

document.getElementById('updateFirmware').addEventListener('click', async () => {
    log('펌웨어 업데이트 버튼 클릭됨');
    try {
        const response = await fetch('https://api.github.com/repos/gnldud15/HEHP_web/releases/latest');
        if (!response.ok) throw new Error('GitHub API 호출 실패');
        
        log('GitHub API 호출 성공');
        const data = await response.json();
        
        if (!data.assets || data.assets.length === 0) {
            throw new Error('펌웨어 자산을 찾을 수 없음');
        }

        const firmwareUrl = data.assets[0].browser_download_url;
        const firmwareResponse = await fetch(firmwareUrl);
        if (!firmwareResponse.ok) throw new Error('펌웨어 다운로드 실패');
        
        log('펌웨어 다운로드 성공');
        const firmwareArrayBuffer = await firmwareResponse.arrayBuffer();
        const firmware = new Uint8Array(firmwareArrayBuffer);
        log('펌웨어 다운로드 완료, 플래싱 시작');
        await flashFirmware(firmware);
        log('펌웨어 업로드 완료');
    } catch (error) {
        log('펌웨어 업로드 실패: ' + error.message);
        console.error('펌웨어 업로드 실패:', error);
    } finally {
        if (port && port.readable) {
            await port.close();
            log('디바이스 연결 해제됨');
        }
    }
});

async function flashFirmware(firmware) {
    const writer = port.writable.getWriter();
    for (let i = 0; i < firmware.length; i += 256) {
        const chunk = firmware.slice(i, i + 256);
        await writer.write(chunk);
    }
    writer.releaseLock();
}
