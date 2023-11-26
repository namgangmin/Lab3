

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SENTENCE_LENGTH 1000

double calcWPM(int strokes, int timeElapsed){
	return (strokes / 5.0) / (timeElapsed / 60.0);
}

int main() {
	printf("타자 연습 프로그램 \n");
	printf("다음 문장을 따라 입력하세요:\n");

	char targetSentences[3][MAX_SENTENCE_LENGTH] = { "동해 물과 백두산이 마르고 닳도록",
	     "하느님이 보우 하사 우리나라 만세",
	     "무궁화 삼천리 화려강산 대한사람 대한으로 길이 보전하세"};

	int totalStrokes = 0;
	int incorrect =  0;

	double sum = 0;

	for (int i = 0; i < 3; ++i){
		printf("\n%s\n", targetSentences[i]);

		time_t startTime = time(NULL);
		char userInput[MAX_SENTENCE_LENGTH];
		fgets(userInput, sizeof(userInput), stdin);

		if (strcmp(targetSentences[i], userInput) != 0){
					incorrect++;
		}
		
		totalStrokes += strlen(targetSentences[i]);
		
		time_t endTime = time(NULL);
		int timeElapsed = (int)difftime(endTime, startTime);

		sum += calcWPM(totalStrokes, timeElapsed);
	}

	double averageWPM = sum/3.0;

	printf("오답 수 : %d\n", incorrect);
	printf("평균 분당 타자 수: %.2f\n", averageWPM);

	return 0;
}

