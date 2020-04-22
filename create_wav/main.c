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


long file_length(FILE *f)
{
  long pos;
  long end;

  pos = ftell(f);
  fseek(f, 0, SEEK_END);
  end = ftell(f);
  fseek(f, pos, SEEK_SET);

  return end;
}


void usage(void )
{
	printf("Usage:\n");
	printf("create_mono_wave.exe <Options>\n");
	//printf("  -c    :  enable option c to create example wave \n");
	//printf("  -v  value  :  enable option v with value \n");
    printf("  -s  value  :  sample rate(Hz) s with value \n");
    printf("  -d  value  :  sample width(bits) d with value \n");
    printf("  -i  value  :  specify input file with file name \n");
    printf("  -o  value  :  specify output file with file name \n");
    printf("=======");
    printf ("requirement: the input data format should one data point per line, prefix with 0x\n");
    printf("example: create_mono_wave.exe -s 32000 -d 32 -i input.txt -o output.wav\n");
    printf("=======");
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

void waveAddSample_fixed_point( Wave* wave, const int32_t* samples ){
    int i;
    short int sample8bit;
    int sample16bit;
    long int sample32bit;
    char* sample;
    if( wave->header.bitsPerSample == 8 ){
        for( i=0; i<wave->header.numChannels; i+= 1){
            sample8bit = (short int)samples[i];
            toLittleEndian(1, (void*) &sample8bit);
            sample = (char*)&sample8bit;
            (wave->data)[ wave->index ] = sample[0];
            wave->index += 1;
        }
    }
    if( wave->header.bitsPerSample == 16 ){
        for( i=0; i<wave->header.numChannels; i+= 1){
            sample16bit =  (int)samples[i] ;// (int) (32767*samples[i]);
            toLittleEndian(2, (void*) &sample16bit);
            sample = (char*)&sample16bit;
            wave->data[ wave->index + 0 ] = sample[0];
            wave->data[ wave->index + 1 ] = sample[1];
            wave->index += 2;
        }
    }
    if( wave->header.bitsPerSample == 32 ){
        for( i=0; i<wave->header.numChannels; i+= 1){
            //sample32bit = (long int) ((pow(2,32-1)-1)*samples[i]);
            sample32bit =  samples[i];
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

int call_example_wave_mono(uint32_t sampleRate,uint32_t numChannels,uint32_t bitsPerSample, int32_t* raw_data, uint32_t nSamples, char* file_name ){
    // Define some variables for the sound
    numChannels= 1; //fixed to 1  for mono case 
    Wave mySound = makeWave((int)sampleRate,numChannels,bitsPerSample);
    long long int totalBytes = (long long int)(nSamples*bitsPerSample/8);
    mySound.data = (char*)malloc(totalBytes);
    mySound.index = 0;
    mySound.size = totalBytes;
    mySound.nSamples = nSamples;
    mySound.header.chunkSize = 4+8+16+8+totalBytes;
    mySound.header.subChunk2Size = totalBytes;
    printf("==================\n");
    printf("size:%lu\n",(long int) mySound.size);
    printf("nSamples:%lu\n",(long int)mySound.nSamples);
    printf("header.chunkSize:%d\n",mySound.header.chunkSize);
    printf("header.subChunk2Size:%d\n",mySound.header.subChunk2Size);
    printf("header.bitsPerSample:%d\n",mySound.header.bitsPerSample);
    printf("==================\n");
    // Add all of the data
#if 0 
    int i;
    float frameData[2];
    for(i=0; i<nSamples; i+=1 ){
        frameData[0] = cos(lFreq*(float)i*3.14159/sampleRate);
        frameData[1] = cos(rFreq*(float)i*3.14159/sampleRate);
        waveAddSample( &mySound, frameData );
    }
#else 
    int i;
    int32_t frameData[1]; //mono case 
    for(i=0; i<nSamples; i+=1 ){
        frameData[0] = raw_data[i] ;
        //printf("frameData[0]:0x%x\n",frameData[0]);
        waveAddSample_fixed_point( &mySound, frameData );
    }

#endif 

    // Write it to a file and clean up when done
    waveToFile( &mySound, file_name);
    waveDestroy( &mySound );

    return 0;
}


int32_t main(int32_t argc, char * argv[])
{
	
	int32_t c_opt = false; 
	int32_t v_opt = false;
	int32_t v_val = 0; 
    int32_t data_len = 0; 
    int32_t s_opt = false; 
    int32_t s_value = 16000; //16k 
    int32_t d_opt = false; 
    int32_t d_value = 32;//32bits
    #define DEFAULT_INPUT_FILE_NAME     "input.txt" 
    char input_file_path[256];
    #define DEFAULT_OUTPUT_FILE_NAME     "output.wav" 
    char output_file_path[256];
    int32_t find_input_file_path = false; 
    int32_t find_output_file_path = false; 
    int32_t i_opt = false; 
    int32_t o_opt = false; 
 do
  {
    fprintf(stdout,"Tool\n");
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
        else if (strcmp(argu_opt, "-s") == 0)
        {
		  s_opt = true ;
		  if((i+1) <argc)
			sscanf(argv[i+1], "%d", &s_value);
	      else
		  {
		    printf("err: missing value with -s option.");
		    usage();
			show_result(false);
			return -1; 
		  }
		}
        else if (strcmp(argu_opt, "-d") == 0)
        {
		  d_opt = true ;
		  if((i+1) <argc)
			sscanf(argv[i+1], "%d", &d_value);
	      else
		  {
		    printf("err: missing value with -d option.");
		    usage();
			show_result(false);
			return -1; 
		  }
		}
        else if (strcmp(argu_opt, "-i") == 0)
        {
		  i_opt = true ;
		  if((i+1) <argc)
          {
            strcpy(input_file_path,argv[i+1]);
            find_input_file_path = true;
          }
	      else
		  {
		    printf("err: missing value with -i option.");
		    usage();
			show_result(false);
			return -1; 
		  }
		}
        else if (strcmp(argu_opt, "-o") == 0)
        {
		  o_opt = true ;
		  if((i+1) <argc)
          {
            strcpy(output_file_path,argv[i+1]);
            find_output_file_path  = true; 
          }
	      else
		  {
		    printf("err: missing value with -o option.");
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
	

    if (find_input_file_path == false)
      strcpy(input_file_path,DEFAULT_INPUT_FILE_NAME);

    if (find_output_file_path == false)
      strcpy(output_file_path,DEFAULT_OUTPUT_FILE_NAME);

	if (c_opt)  printf(" c_opt is enabled\n");
	
	if (v_opt)  printf(" v_opt is enabled with value 0x%x\n",v_val);
	//InitShowProgress();
	//call_example_wave_1();
	//call_example_wave_2();
	//call_example_wave_3();
//read data from external txt file  
 #define  READ_RAW_DATA_FROM_FILE  1 
 #if READ_RAW_DATA_FROM_FILE
  FILE *fp;

  printf("\n========read data from file======= \n");
  fp = fopen(input_file_path, "r");
  if (fp == NULL)
  {
    printf("ERROR: File RawdataforWave_Input.txt not found");
    return -1; 
  }
  long byte_length = 0x0; 
  byte_length = file_length(fp);

  int32_t data_int =0;
  char  data_str[100];
  int32_t line_cnt = 0; 
  while(fscanf(fp,"%s", data_str) ==1) 
  {
     line_cnt ++ ; 
  }
  printf("line_cnt:%d\n",line_cnt);
  int32_t *SampleData = (int32_t*)malloc(line_cnt*sizeof(int32_t)); 
  if(SampleData == NULL)
  {
      printf("ERROR: Malloc failure for input_data_buffer\n");
      return -1; 
  }
  int32_t input_data_idx = 0 ;
  fseek( fp, 0, SEEK_SET );//rest to head of file 
  while(fscanf(fp,"%s", data_str) ==1) // while(ret!=EOF)
  {
    sscanf(data_str,"0x%x",&data_int);
    //printf("%s\n", data_str);
    //printf("0x%x\n", data_int); 
    SampleData[input_data_idx++] = data_int; 
  }; 

  uint32_t nSamples  = input_data_idx ;
  printf("data_cnt:%d\n",nSamples);

  fclose(fp);
#endif  

//generate the example mono data fixed-point 32 bit 

#define  GENERATE_RAW_DATA 0 
#if GENERATE_RAW_DATA
    float gen_sampleRate = 96000.0; // hertz
    float lFreq =  880.0;       // hertz
    #define NUM_SAMPLES  10000
    uint32_t nSamples  = NUM_SAMPLES; //num of datas
    int32_t SampleData[NUM_SAMPLES]; //mono case 
#define DUMP_RAW_DATA_TXT 1
#if DUMP_RAW_DATA_TXT
    FILE *file;
    char* filename = "RawdataforWave.txt";
    file = fopen(filename, "wt");
#endif 
    for(int32_t ii=0; ii<nSamples; ii+=1 ){
        SampleData[ii] =(int32_t) ((float)((pow(2,32-1)-1)*cos(lFreq*(float)ii*3.14159/gen_sampleRate)));
        //printf("SampleData[%d]=0x%x\n",ii, SampleData[ii]);
#if DUMP_RAW_DATA_TXT
        fprintf( file,"0x%x\n", SampleData[ii]);
#endif 
    }

#if DUMP_RAW_DATA_TXT
    fclose( file );
#endif 

#endif 

#define DUMP_RAW_DATA_BIN 0 
#if DUMP_RAW_DATA_BIN
    FILE *file;
    char* filename = "RawdataforWave.bin";
    file = fopen(filename, "wb");
    fwrite( (void*)(SampleData), sizeof(char), nSamples*4, file );
    fclose( file );
#endif 
     
    uint32_t sampleRate = s_value;
    uint32_t dataWidth = d_value; 
    uint32_t wavChannels = 1 ;//fixed to mono data 
    call_example_wave_mono(sampleRate,wavChannels,dataWidth,SampleData,nSamples,output_file_path);
	//ShowProgress(10,10);
	show_result(true);
	return 0;
	
  }while(0);
   
  show_result(false);
  return 0 ;
  
}
