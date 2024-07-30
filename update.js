let port;
let esptool;
let logElement = document.getElementById('log');

const log = (message) => {
    logElement.textContent += message + '\n';
};

// ESPLoader 클래스가 정의되어 있는지 확인
console.log('ESPLoader:', typeof ESPLoader);

document.getElementById('connect').addEventListener('click', () => {
    log('디바이스 연결 버튼 클릭됨');
    navigator.serial.requestPort()
        .then(selectedPort => {
            port = selectedPort;
            log('시리얼 포트 요청 성공');
            return port.open({ baudRate: 115200 });
        })
        .then(() => {
            log('시리얼 포트 열림');
            esptool = new ESPLoader(port);
            return esptool.initialize();
        })
        .then(() => {
            log('ESP32 초기화 완료');
            document.getElementById('updateFirmware').disabled = false;
            log('업데이트 버튼 활성화');
        })
        .catch(error => {
            log('디바이스 연결 실패: ' + error.message);
        });
});

document.getElementById('updateFirmware').addEventListener('click', () => {
    log('펌웨어 업데이트 버튼 클릭됨');
    fetch('https://api.github.com/repos/gnldud15/HEHP_web/releases/latest')
        .then(response => {
            log('GitHub API 호출 성공');
            return response.json();
        })
        .then(data => {
            const firmwareUrl = data.assets[0].browser_download_url;
            return fetch(firmwareUrl);
        })
        .then(firmwareResponse => {
            log('펌웨어 다운로드 성공');
            return firmwareResponse.arrayBuffer();
        })
        .then(firmwareArrayBuffer => {
            const firmware = new Uint8Array(firmwareArrayBuffer);
            log('펌웨어 다운로드 완료, 플래싱 시작');
            return esptool.flashData(firmware, 0x1000);
        })
        .then(() => {
            log('펌웨어 업로드 완료');
        })
        .catch(error => {
            log('펌웨어 업로드 실패: ' + error.message);
        })
        .finally(() => {
            if (port && port.readable) {
                port.close().then(() => {
                    log('디바이스 연결 해제됨');
                });
            }
        });
});
