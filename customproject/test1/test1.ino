#include <FastLED.h>
#include <SoftwareSerial.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 5
#define NUM_LEDS 60
#define BRIGHTNESS 32

SoftwareSerial mySerial(0, 1); // RX, TX

CRGB leds[NUM_LEDS];
int color[NUM_LEDS];
int numbers[10] = {1,2,3,4,5,6,7,8,9,10};
unsigned char serialReadCount = 0;
unsigned char incomingByte = 0;

int bubbleCnt = 0;
int insertCnt = 0;

void displayColor(){
// display color
for(int k = 0; k < NUM_LEDS; k++) {
  leds[k] = CHSV(color[k], 255, 255);
  FastLED.show();
}
}


//void blinkColor() {
//
//   FastLED.show();
//}

void bubbleSort() {
  Serial.println("In bubblesort");

  int color[NUM_LEDS];

  // assigns random colors to each element in the LED strip
  for(int i = 0; i < NUM_LEDS; i++) {
    color[i] = random(0,256);
    leds[i] = CHSV(color[i], 255, 255);
   }

   FastLED.show();
   delay(1000);

    for(int x = 0; x < NUM_LEDS; x++) {
      for(int i = 0; i < NUM_LEDS - 1 -x; i++) {

        // comparisons and swap
        if(color[i] > color[i+1]) {
          int temp = color[i];
          color[i] = color[i+1];
          color[i+1] = temp;
         }

        // display color
       for(int j = 0; j < NUM_LEDS; j++) {
         leds[j] = CHSV(color[j], 255, 255);
          FastLED.show();
        }
      }
    }
  }

void insertionSort() {
    Serial.println("In insertion sort");

  // assigns random colors to each element in the LED strip
  for(int z = 0; z < NUM_LEDS; z++) {
    color[z] = random(0,256);
    leds[z] = CHSV(color[z], 255, 255);
   }

   FastLED.show();
   delay(1000);

    int i, key, j;
    for (i = 1; i < NUM_LEDS; i++) {
        key = color[i];
        j = i - 1;

        /* Move elements of arr[0..i-1], that are
        greater than key, to one position ahead
        of their current position */
        while (j >= 0 && color[j] > key)
        {
            color[j + 1] = color[j];
            j = j - 1;


        }
        displayColor();

        color[j + 1] = key;
    }


}

void cocktailSort() {
  mySerial.println("In cocktail sort");

  // assigns random colors to each element in the LED strip
  for(int z = 0; z < NUM_LEDS; z++) {
    color[z] = random(0,256);
    leds[z] = CHSV(color[z], 255, 255);
   }

   FastLED.show();
   delay(1000);

    bool swapped = true;
    int start = 0;
    int end = NUM_LEDS - 1;

    while (swapped) {
        // reset the swapped flag on entering
        // the loop, because it might be true from
        // a previous iteration.
        swapped = false;

        // loop from left to right same as
        // the bubble sort
        for (int i = start; i < end; ++i) {
            if (color[i] > color[i + 1]) {
                int temp = color[i];
                color[i] = color[i+1];
                color[i+1] = temp;
                swapped = true;
                displayColor();

            }
        }

        // if nothing moved, then array is sorted.
        if (!swapped)
            break;

        // otherwise, reset the swapped flag so that it
        // can be used in the next stage
        swapped = false;

        // move the end point back by one, because
        // item at the end is in its rightful spot
        --end;

        // from right to left, doing the
        // same comparison as in the previous stage
        for (int i = end - 1; i >= start; --i) {
            if (color[i] > color[i + 1]) {
                int temp = color[i];
                color[i] = color[i+1];
                color[i+1] = temp;
                swapped = true;
                displayColor();
            }
        }


        // increase the starting point, because
        // the last stage would have moved the next
        // smallest number to its rightful spot.
        ++start;
    }

    // display color
        for(int k = 0; k < NUM_LEDS; k++) {
          leds[k] = CHSV(color[k], 255, 255);
          FastLED.show();
        }
}


// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int color[], int l, int m, int r) {
  Serial.println("In merge sort");

    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */
    int L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++) {
        L[i] = color[l + i];
      }

    for (j = 0; j < n2; j++) {
        R[j] = color[m + 1+ j];
      }


    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            color[k] = L[i];
            i++;
        }
        else
        {
            color[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        color[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        color[k] = R[j];
        j++;
        k++;
    }
}

/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void mergeSort(int color[], int l, int r) {
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;

        // Sort first and second halves
        mergeSort(color, l, m);
        mergeSort(color, m+1, r);

        merge(color, l, m, r);
    }
}


void chooseSort() {
  }


void setup() {
  //setup safety
  //delay(1000);

  pinMode(13, OUTPUT);
  digitalWrite(13,LOW);

   Serial.begin(9600);
   mySerial.begin(9600);

   // Initializes the led strip
   FastLED.addLeds<WS2812B, PIN, GRB>(leds, NUM_LEDS);
   FastLED.setBrightness(  BRIGHTNESS );

   int color[NUM_LEDS];

   // assigns random colors to each element in the LED strip
   for(int i = 0; i < NUM_LEDS; i++) {
     color[i] = random(0,256);
     leds[i] = CHSV(color[i], 255, 255);
    }

    FastLED.show();
}

unsigned char select_bubble = 0x00;
int select_insertion = 255;
int select_cocktail = 255;

void loop() {
  if (Serial.available()) {
    //Serial.println("Serial is available: ");
    incomingByte = Serial.read();
    Serial.print("incomingByte is: ");
    Serial.println(incomingByte, DEC);
    if(incomingByte == 0x0001 && bubbleCnt == 0) {
      //Serial.println("sort");
      //mergeSort(color, 0, NUM_LEDS);
    //  mergeSort(color, 0, NUM_LEDS);
  //  if(isRunning) {
      bubbleCnt++;
     bubbleSort();
    //  isRunning = false;
    //}
    //  incomingByte = 0;
  } else if(incomingByte == 0x0002 && insertCnt == 0) {
    insertCnt++;
    insertionSort();
  }
    else{
      Serial.println("nothing pressed");
    }


  //  delay(200);
  }
  else {
    Serial.println("Serial is NOT available");
  }
}
