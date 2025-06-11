#include <Adafruit_CircuitPlayground.h>

int roundNum = 0;
int score = 0;
int maxRounds = 5;
bool gameActive = false;
bool waitingForInput = false;
int targetLED = 0;
String targetColor = "green";
unsigned long reactionStart;
unsigned long reactionDelay = 1500; // 1.5 sec delay for each round

void setup() {
  CircuitPlayground.begin();
  CircuitPlayground.clearPixels();
  CircuitPlayground.setBrightness(50);
  Serial.begin(9600);
}

void loop() {
  if (!gameActive) {
    idleAnimation();
    if (CircuitPlayground.slideSwitch()) {
      startGame();
    }
  } else if (waitingForInput) {
    checkInput();
  }
}

void idleAnimation() {
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 0, 0, 30);
  }
  delay(300);
  CircuitPlayground.clearPixels();
  delay(300);
}

void startGame() {
  CircuitPlayground.clearPixels();
  roundNum = 0;
  score = 0;
  gameActive = true;
  delay(500);
  nextRound();
}

void nextRound() {
  roundNum++;
  if (roundNum > maxRounds) {
    endGame();
    return;
  }

  targetLED = random(0, 10);
  targetColor = (random(0, 2) == 0) ? "green" : "blue";

  if (targetColor == "green") {
    CircuitPlayground.setPixelColor(targetLED, 0, 255, 0);
  } else {
    CircuitPlayground.setPixelColor(targetLED, 0, 0, 255);
  }

  reactionStart = millis();
  waitingForInput = true;
}

void checkInput() {
  if (millis() - reactionStart > reactionDelay) {
    timeoutInput();
    return;
  }

  bool isLeftSide = targetLED < 5;
  bool leftPressed = CircuitPlayground.leftButton();
  bool rightPressed = CircuitPlayground.rightButton();

  if (targetColor == "green") {
    if ((isLeftSide && leftPressed) || (!isLeftSide && rightPressed)) {
      correctInput();
    } else if (leftPressed || rightPressed) {
      incorrectInput();
    }
  } else if (targetColor == "blue") {
    if ((isLeftSide && rightPressed) || (!isLeftSide && leftPressed)) {
      correctInput();
    } else if (leftPressed || rightPressed) {
      incorrectInput();
    }
  }
}

void correctInput() {
  waitingForInput = false;
  score++;
  CircuitPlayground.setPixelColor(targetLED, 255, 255, 255);
  delay(300);
  CircuitPlayground.clearPixels();
  delay(300);
  nextRound();
}

void incorrectInput() {
  waitingForInput = false;
  CircuitPlayground.setPixelColor(targetLED, 255, 0, 0);
  delay(500);
  CircuitPlayground.clearPixels();
  delay(200);
  nextRound();
}

void timeoutInput() {
  waitingForInput = false;
  CircuitPlayground.setPixelColor(targetLED, 255, 165, 0);
  delay(500);
  CircuitPlayground.clearPixels();
  delay(200);
  nextRound();
}

void endGame() {
  gameActive = false;
  CircuitPlayground.clearPixels();
  for (int i = 0; i < score && i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 255, 255, 0);
  }
  delay(4000);
  CircuitPlayground.clearPixels();
  
}