#include <stdint.h> 
#include <stdio.h> 
#include <string.h>
#include <malloc.h>
#include <math.h>

#define true 1 
#define false 0 
#define bool int32_t
//show progress 
#define InitShowProgress()  fprintf(stdout, "Processing ... %3d%%", 0); fflush(stdout);
#define ShowProgress(curPos,MaxPos) fprintf(stdout, "\b\b\b\b%3d%%", (char) (((curPos) *100)/MaxPos) ); fflush(stdout);

struct message_s
{
	int32_t   cmd:6;			
	uint32_t  err:1;			
	uint32_t  repl:1;			
	uint32_t  blk:8;			
	uint32_t  len:16;			
	uint32_t  crc;			       
} ;			       

#define SIZE(a) (int32_t)(sizeof(a) / sizeof(a[0]))

//show pass and failure 
static  char * szPass[] = {
  "                                        \n",
  "  ########     ###     ######   ######  \n",
  "  ##     ##   ## ##   ##    ## ##    ## \n",
  "  ##     ##  ##   ##  ##       ##       \n",
  "  ########  ##     ##  ######   ######  \n",
  "  ##        #########       ##       ## \n",
  "  ##        ##     ## ##    ## ##    ## \n",
  "  ##        ##     ##  ######   ######  \n",
};

static const char * szFail[] = {
  "                          \n",
  "   ######   ##   # #      \n",
  "   #       #  #  # #      \n",
  "   #####  #    # # #      \n",
  "   #      ###### # #      \n",
  "   #      #    # # #      \n",
  "   #      #    # # ###### \n",
};


void show_result(bool p)
{
  if (p)
  {
    //set_txt_color(COLOR_PASS);
    for (int32_t i = 0; i < SIZE(szPass); i++)
    {
      fprintf(stdout, "%s", szPass[i]);
    }
  }
  else
  {
    //set_txt_color(COLOR_ERROR);
    for (int32_t i = 0; i < SIZE(szFail); i++)
    {
      fprintf(stdout, "%s", szFail[i]);
    }
  }
  //set_txt_color(COLOR_NORMAL);
}



int32_t endianess_is_little()
{
    uint32_t endianness = 1;
    return ((char*)(&endianness))[0]==1;

}

void delay(uint32_t cnt)
{
	while(cnt>0)
		cnt--;
}

void usage(void )
{
	printf("Usage:\n");
	printf("demo_tool.exe <Options>\n");
	printf("  -c    :  enable option c to create example wave \n");
	printf("  -v  value  :  enable option v with value \n");
}


//http://blog.acipo.com/generating-wave-files-in-c/

// -------------------------------------------------- [ Section: Endianness ] -
int isBigEndian() {
    int test = 1;
    char *p = (char*)&test;

    return p[0] == 0;
}
void reverseEndianness(const long long int size, void* value){
    int i;
    char result[32];
    for( i=0; i<size; i+=1 ){
        result[i] = ((char*)value)[size-i-1];
    }
    for( i=0; i<size; i+=1 ){
        ((char*)value)[i] = result[i];
    }
}
void toBigEndian(const long long int size, void* value){
    char needsFix = !( (1 && isBigEndian()) || (0 && !isBigEndian()) );
    if( needsFix ){
        reverseEndianness(size,value);
    }
}
void toLittleEndian(const long long int size, void* value){
    char needsFix = !( (0 && isBigEndian()) || (1 && !isBigEndian()) );
    if( needsFix ){
        reverseEndianness(size,value);
    }
}

// ------------------------------------------------- [ Section: Wave Header ] -
typedef struct WaveHeader {
    // Riff Wave Header
    char chunkId[4];
    int  chunkSize;
    char format[4];

    // Format Subchunk
    char subChunk1Id[4];
    int  subChunk1Size;
    short int audioFormat;
    short int numChannels;
    int sampleRate;
    int byteRate;
    short int blockAlign;
    short int bitsPerSample;
    //short int extraParamSize;

    // Data Subchunk
    char subChunk2Id[4];
    int  subChunk2Size;

} WaveHeader;

WaveHeader makeWaveHeader(int const sampleRate, short int const numChannels, short int const bitsPerSample ){
    WaveHeader myHeader;

    // RIFF WAVE Header
    myHeader.chunkId[0] = 'R';
    myHeader.chunkId[1] = 'I';
    myHeader.chunkId[2] = 'F';
    myHeader.chunkId[3] = 'F';
    myHeader.format[0] = 'W';
    myHeader.format[1] = 'A';
    myHeader.format[2] = 'V';
    myHeader.format[3] = 'E';

    // Format subchunk
    myHeader.subChunk1Id[0] = 'f';
    myHeader.subChunk1Id[1] = 'm';
    myHeader.subChunk1Id[2] = 't';
    myHeader.subChunk1Id[3] = ' ';
    myHeader.audioFormat = 1; // FOR PCM
    myHeader.numChannels = numChannels; // 1 for MONO, 2 for stereo
    myHeader.sampleRate = sampleRate; // ie 44100 hertz, cd quality audio
    myHeader.bitsPerSample = bitsPerSample; // 
    myHeader.byteRate = myHeader.sampleRate * myHeader.numChannels * myHeader.bitsPerSample / 8;
    myHeader.blockAlign = myHeader.numChannels * myHeader.bitsPerSample/8;

    // Data subchunk
    myHeader.subChunk2Id[0] = 'd';
    myHeader.subChunk2Id[1] = 'a';
    myHeader.subChunk2Id[2] = 't';
    myHeader.subChunk2Id[3] = 'a';

    // All sizes for later:
    // chuckSize = 4 + (8 + subChunk1Size) + (8 + subChubk2Size)
    // subChunk1Size is constanst, i'm using 16 and staying with PCM
    // subChunk2Size = nSamples * nChannels * bitsPerSample/8
    // Whenever a sample is added:
    //    chunkSize += (nChannels * bitsPerSample/8)
    //    subChunk2Size += (nChannels * bitsPerSample/8)
    myHeader.chunkSize = 4+8+16+8+0;
    myHeader.subChunk1Size = 16;
    myHeader.subChunk2Size = 0;
    
    return myHeader;
}

// -------------------------------------------------------- [ Section: Wave ] -
typedef struct Wave {
    WaveHeader header;
    char* data;
    long long int index;
    long long int size;
    long long int nSamples;
} Wave;

Wave makeWave(int const sampleRate, short int const numChannels, short int const bitsPerSample){
    Wave myWave;
    myWave.header = makeWaveHeader(sampleRate,numChannels,bitsPerSample);
    return myWave;
}
void waveDestroy( Wave* wave ){
    free( wave->data );
}
void waveSetDuration( Wave* wave, const float seconds ){
    long long int totalBytes = (long long int)(wave->header.byteRate*seconds);
    wave->data = (char*)malloc(totalBytes);
    wave->index = 0;
    wave->size = totalBytes;
    wave->nSamples = (long long int) wave->header.numChannels * wave->header.sampleRate * seconds;
    wave->header.chunkSize = 4+8+16+8+totalBytes;
    wave->header.subChunk2Size = totalBytes;
}
void waveAddSample( Wave* wave, const float* samples ){
    int i;
    short int sample8bit;
    int sample16bit;
    long int sample32bit;
    char* sample;
    if( wave->header.bitsPerSample == 8 ){
        for( i=0; i<wave->header.numChannels; i+= 1){
            sample8bit = (short int) (127+127.0*samples[i]);
            toLittleEndian(1, (void*) &sample8bit);
            sample = (char*)&sample8bit;
            (wave->data)[ wave->index ] = sample[0];
            wave->index += 1;
        }
    }
    if( wave->header.bitsPerSample == 16 ){
        for( i=0; i<wave->header.numChannels; i+= 1){
            sample16bit = (int) (32767*samples[i]);
            //sample = (char*)&litEndianInt( sample16bit );
            toLittleEndian(2, (void*) &sample16bit);
            sample = (char*)&sample16bit;
            wave->data[ wave->index + 0 ] = sample[0];
            wave->data[ wave->index + 1 ] = sample[1];
            wave->index += 2;
        }
    }
    if( wave->header.bitsPerSample == 32 ){
        for( i=0; i<wave->header.numChannels; i+= 1){
            sample32bit = (long int) ((pow(2,32-1)-1)*samples[i]);
            //sample = (char*)&litEndianLong( sample32bit );
            toLittleEndian(4, (void*) &sample32bit);
            sample = (char*)&sample32bit;
            wave->data[ wave->index + 0 ] = sample[0];
            wave->data[ wave->index + 1 ] = sample[1];
            wave->data[ wave->index + 2 ] = sample[2];
            wave->data[ wave->index + 3 ] = sample[3];
            wave->index += 4;
        }
    }
}
void waveToFile( Wave* wave, const char* filename ){

    // First make sure all numbers are little endian
    toLittleEndian(sizeof(int), (void*)&(wave->header.chunkSize));
    toLittleEndian(sizeof(int), (void*)&(wave->header.subChunk1Size));
    toLittleEndian(sizeof(short int), (void*)&(wave->header.audioFormat));
    toLittleEndian(sizeof(short int), (void*)&(wave->header.numChannels));
    toLittleEndian(sizeof(int), (void*)&(wave->header.sampleRate));
    toLittleEndian(sizeof(int), (void*)&(wave->header.byteRate));
    toLittleEndian(sizeof(short int), (void*)&(wave->header.blockAlign));
    toLittleEndian(sizeof(short int), (void*)&(wave->header.bitsPerSample));
    toLittleEndian(sizeof(int), (void*)&(wave->header.subChunk2Size));

    // Open the file, write header, write data
    FILE *file;
    file = fopen(filename, "wb");
    fwrite( &(wave->header), sizeof(WaveHeader), 1, file );
    fwrite( (void*)(wave->data), sizeof(char), wave->size, file );
    fclose( file );

    // Convert back to system endian-ness
    toLittleEndian(sizeof(int), (void*)&(wave->header.chunkSize));
    toLittleEndian(sizeof(int), (void*)&(wave->header.subChunk1Size));
    toLittleEndian(sizeof(short int), (void*)&(wave->header.audioFormat));
    toLittleEndian(sizeof(short int), (void*)&(wave->header.numChannels));
    toLittleEndian(sizeof(int), (void*)&(wave->header.sampleRate));
    toLittleEndian(sizeof(int), (void*)&(wave->header.byteRate));
    toLittleEndian(sizeof(short int), (void*)&(wave->header.blockAlign));
    toLittleEndian(sizeof(short int), (void*)&(wave->header.bitsPerSample));
    toLittleEndian(sizeof(int), (void*)&(wave->header.subChunk2Size));
}

// -------------------------------------------------------- [ Section: Main ] -
int call_example_wave_1(){
    // Define some variables for the sound
    float sampleRate = 44100.0; // hertz
    float freq = 440.0;         // hertz
    float duration = 0.5;       // seconds

    int nSamples = (int)(duration*sampleRate);
    
    // Create a mono (1), 32-bit sound and set the duration
    Wave mySound = makeWave((int)sampleRate,1,32);
    waveSetDuration( &mySound, duration );

    // Add all of the data
    int i;
    float frameData[1];
    for(i=0; i<nSamples; i+=1 ){
        frameData[0] = cos(freq*(float)i*3.14159/sampleRate);
        waveAddSample( &mySound, frameData );
    }

    // Write it to a file and clean up when done
    waveToFile( &mySound, "mono-32bit.wav");
    waveDestroy( &mySound );

    return 0;
}


//This one writes an 8-bit stereo sound.
int call_example_wave_2(){
    // Define some variables for the sound
    float sampleRate = 44100.0; // hertz
    float lFreq =  880.0;       // hertz
    float rFreq = 1760.0;       // hertz
    float duration = 1.0;       // seconds

    int nSamples = (int)(duration*sampleRate);
    
    // Create a stereo (2), 8-bit sound and set the duration
    Wave mySound = makeWave((int)sampleRate,2,32);
    waveSetDuration( &mySound, duration );

    // Add all of the data
    int i;
    float frameData[2];
    for(i=0; i<nSamples; i+=1 ){
        frameData[0] = cos(lFreq*(float)i*3.14159/sampleRate);
        frameData[1] = cos(rFreq*(float)i*3.14159/sampleRate);
        waveAddSample( &mySound, frameData );
    }

    // Write it to a file and clean up when done
    waveToFile( &mySound, "stereo-8bit.wav");
    waveDestroy( &mySound );

    return 0;
}

//1 second 880 hertz tone in a mono 16 bit file at 96000 hertz:
int call_example_wave_3(){
    // Define some variables for the sound
    float sampleRate = 96000.0; // hertz
    float lFreq =  880.0;       // hertz
    float duration = 1.0;       // seconds

    int nSamples = (int)(duration*sampleRate);
    
    // Create a mono (1), 16-bit sound and set the duration
    Wave mySound = makeWave((int)sampleRate,1,16);
    waveSetDuration( &mySound, duration );

    // Add all of the data
    int i;
    float frameData[1];
    for(i=0; i<nSamples; i+=1 ){
        frameData[0] = cos(lFreq*(float)i*3.14159/sampleRate);
        waveAddSample( &mySound, frameData );
    }

    // Write it to a file and clean up when done
    waveToFile( &mySound, "mono-16bit.wav");
    waveDestroy( &mySound );

    return 0;
}

int32_t main(int32_t argc, char * argv[])
{
	
	int32_t c_opt = false; 
	int32_t v_opt = false;
	int32_t v_val = 0; 
 do
  {
    fprintf(stdout,"demo tool\n\n");
    if (argc > 1)
    {
      for (int32_t i=1; i < argc; i++) 
	  {
        char *argu_opt = NULL;
        argu_opt = argv[i];
        if (strcmp(argu_opt, "-c") == 0)
        {
          c_opt = true;
        } 
		else if (strcmp(argu_opt, "-v") == 0)
        {
		  v_opt = true ;
		  if((i+1) <argc)
			sscanf(argv[i+1], "%d", &v_val);
	      else
		  {
		    printf("err: missing value with -v option.");
		    usage();
			show_result(false);
			return -1; 
		  }
		}
		else if (strcmp(argu_opt, "-h") == 0)
        {
		  usage();
		  return 0; 
          
	    }
	  }
	}
	else
	{
		usage();
	}
	
	if (c_opt)  printf(" c_opt is enabled\n");
	
	if (v_opt)  printf(" v_opt is enabled with value 0x%x\n",v_val);
	InitShowProgress();
	delay(100000);
	ShowProgress(5,10);
	delay(100000);
	ShowProgress(10,10);
	call_example_wave_1();
	call_example_wave_2();
	call_example_wave_3();
	show_result(true);
	return 0;
	
  }while(0);
   
  show_result(false);
  return 0 ;
  
}
