#define CLK 2    // 2번핀을 CLK에 연결
#define DT 3     // 3번핀을 DT에 연결
#define stepp 4 
#define dir 5 // 이상 모터 작동 관련
#define CDS A2

int count = 0;   // 카운팅 저장용 변수
int currentCLK;     // 현재 CLK의 상태 저장용 변수
int lastCLK;         // 이전 CLK의 상태 저장용 변수
String currentDir ="";     // 현재 방향 출력용 문자열 변수
int len;
int code;

void setup() {
  
  // 엔코더,CDS 핀을 입력으로 설정
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  pinMode(CDS,INPUT);


  // 시리얼 출력 개시
  Serial.begin(9600);

  // CLK핀의 최초 상태 저장
  lastCLK = digitalRead(CLK);
  
  //아두이노 인터럽트 설정
  attachInterrupt(0, updateEncoder, CHANGE);
  attachInterrupt(1, updateEncoder, CHANGE);

  //모터 작동 관련
   pinMode(stepp,OUTPUT);
   pinMode(dir,OUTPUT);
}

void loop() {
  digitalWrite(dirpin,direct()); // direct는 다른 tab에서 결정
  for (int i = 0; i < steps(); i++) //이동 정도 지정 steps(다른 tab에서 결정)에 저장된 만큼 이동함
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }
}

void updateEncoder(){   // 인터럽트 발생시 실행되는 함수
  // CLK의 현재 상태를 읽어서 
  currentCLK = digitalRead(CLK);

  // CLK핀의 신호가 바뀌었고(즉, 로터리엔코더의 회전이 발생했했고), 그 상태가 HIGH이면(최소 회전단위의 회전이 발생했다면) 
  if ((currentCLK != lastCLK)  && currentCLK == 0){

    // DT핀의 신호를 확인해서 엔코더의 회전 방향을 확인함.
    if (digitalRead(DT) != currentCLK) {    // 신호가 다르다면 시계방향 회전
      count ++;                                // 카운팅 용 숫자 1 증가
      currentDir ="시계방향 회전";
    } 
    else {                                   // 신호가 같다면 반시계방향 회전
      count --;                              // 카운팅 용 숫자 1 감소
      currentDir ="반시계 방향 회전";
    }

    Serial.print("회전방향: ");
    Serial.print(currentDir);               // 회전방향 출력
    Serial.print(" | Counter: ");
    Serial.print(count);              // 카운팅 출력
    Serial.print(" | cds: ");
    Serial.println(analogRead(CDS));//조도센서값 출력
  }

  // 마지막 상태 변수 저장
  lastCLK = currentCLK;
  delay(1); //인코더 측정 오류 방지용 딜레이
}
