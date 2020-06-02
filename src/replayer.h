
// Woohoo!
#define MAX_CHANNELS 16

#define Period2Freq(period) ((3546897.f * 65536.f) / (period)) 

struct hvl_envelope
{
  short aFrames, aVolume;
  short dFrames, dVolume;
  short sFrames;
  short rFrames, rVolume;
  short pad;
};

struct hvl_plsentry
{
  UBYTE ple_Note;
  UBYTE ple_Waveform;
  short ple_Fixed;
  signed char  ple_FX[2];
  signed char  ple_FXParam[2];
};

struct hvl_plist
{
  short                pls_Speed;
  short                pls_Length;
  struct hvl_plsentry *pls_Entries;
};

struct hvl_instrument
{
  UBYTE		      ins_Name[128];
  UBYTE               ins_Volume;
  UBYTE               ins_WaveLength;
  UBYTE               ins_FilterLowerLimit;
  UBYTE               ins_FilterUpperLimit;
  UBYTE               ins_FilterSpeed;
  UBYTE               ins_SquareLowerLimit;
  UBYTE               ins_SquareUpperLimit;
  UBYTE               ins_SquareSpeed;
  UBYTE               ins_VibratoDelay;
  UBYTE               ins_VibratoSpeed;
  UBYTE               ins_VibratoDepth;
  UBYTE               ins_HardCutRelease;
  UBYTE               ins_HardCutReleaseFrames;
  struct hvl_envelope ins_Envelope;
  struct hvl_plist    ins_PList;
};

struct hvl_position
{
  UBYTE pos_Track[MAX_CHANNELS];
  signed char  pos_Transpose[MAX_CHANNELS];
};

struct hvl_step
{
  UBYTE stp_Note;
  UBYTE stp_Instrument;
  UBYTE stp_FX;
  UBYTE stp_FXParam;
  UBYTE stp_FXb;
  UBYTE stp_FXbParam;
};

struct hvl_voice
{
  short                  vc_Track;
  short                  vc_NextTrack;
  short                  vc_Transpose;
  short                  vc_NextTranspose;
  short                  vc_OverrideTranspose; // 1.5
  int                  vc_ADSRVolume;
  struct hvl_envelope    vc_ADSR;
  struct hvl_instrument *vc_Instrument;
  ULONG                 vc_SamplePos;
  ULONG                 vc_Delta;
  UWORD                 vc_InstrPeriod;
  UWORD                 vc_TrackPeriod;
  UWORD                 vc_VibratoPeriod;
  UWORD                 vc_WaveLength;
  short                  vc_NoteMaxVolume;
  UWORD                 vc_PerfSubVolume;
  UBYTE                  vc_NewWaveform;
  UBYTE                  vc_Waveform;
  UBYTE                  vc_PlantPeriod;
  UBYTE                  vc_VoiceVolume;
  UBYTE                  vc_PlantSquare;
  UBYTE                  vc_IgnoreSquare;
  UBYTE                  vc_FixedNote;
  short                  vc_VolumeSlideUp;
  short                  vc_VolumeSlideDown;
  short                  vc_HardCut;
  UBYTE                  vc_HardCutRelease;
  short                  vc_HardCutReleaseF;
  UBYTE                  vc_PeriodSlideOn;
  short                  vc_PeriodSlideSpeed;
  short                  vc_PeriodSlidePeriod;
  short                  vc_PeriodSlideLimit;
  short                  vc_PeriodSlideWithLimit;
  short                  vc_PeriodPerfSlideSpeed;
  short                  vc_PeriodPerfSlidePeriod;
  UBYTE                  vc_PeriodPerfSlideOn;
  short                  vc_VibratoDelay;
  short                  vc_VibratoSpeed;
  short                  vc_VibratoCurrent;
  short                  vc_VibratoDepth;
  short                  vc_SquareOn;
  short                  vc_SquareInit;
  short                  vc_SquareWait;
  short                  vc_SquareLowerLimit;
  short                  vc_SquareUpperLimit;
  short                  vc_SquarePos;
  short                  vc_SquareSign;
  short                  vc_SquareSlidingIn;
  short                  vc_SquareReverse;
  UBYTE                  vc_FilterOn;
  UBYTE                  vc_FilterInit;
  short                  vc_FilterWait;
  short                  vc_FilterSpeed;
  short                  vc_FilterUpperLimit;
  short                  vc_FilterLowerLimit;
  short                  vc_FilterPos;
  short                  vc_FilterSign;
  short                  vc_FilterSlidingIn;
  short                  vc_IgnoreFilter;
  short                  vc_PerfCurrent;
  short                  vc_PerfSpeed;
  short                  vc_PerfWait;
  struct hvl_plist      *vc_PerfList;
  signed char                  *vc_AudioPointer;
  signed char                  *vc_AudioSource;
  UBYTE                  vc_NoteDelayOn;
  UBYTE                  vc_NoteCutOn;
  short                  vc_NoteDelayWait;
  short                  vc_NoteCutWait;
  short                  vc_AudioPeriod;
  short                  vc_AudioVolume;
  int                  vc_WNRandom;
  signed char                  *vc_MixSource;
  signed char                   vc_SquareTempBuffer[0x80];
  signed char                   vc_VoiceBuffer[0x282*4];
  UBYTE                  vc_VoiceNum;
  UBYTE                  vc_TrackMasterVolume;
  UBYTE                  vc_TrackOn;
  short                  vc_VoicePeriod;
  ULONG                 vc_Pan;
  ULONG                 vc_SetPan;   // New for 1.4
  ULONG                 vc_PanMultLeft;
  ULONG                 vc_PanMultRight;
  ULONG                 vc_RingSamplePos;
  ULONG                 vc_RingDelta;
  signed char                  *vc_RingMixSource;
  UBYTE                  vc_RingPlantPeriod;
  short                  vc_RingInstrPeriod;
  short                  vc_RingBasePeriod;
  short                  vc_RingAudioPeriod;
  signed char                  *vc_RingAudioSource;
  UBYTE                  vc_RingNewWaveform;
  UBYTE                  vc_RingWaveform;
  UBYTE                  vc_RingFixedPeriod;
  signed char                   vc_RingVoiceBuffer[0x282*4];
};

struct hvl_tune
{
  UBYTE			ht_Name[128];
  UWORD                 ht_SongNum;
  ULONG                 ht_Frequency;
  double                ht_FreqF;
  signed char                  *ht_WaveformTab[MAX_CHANNELS];
  UWORD                 ht_Restart;
  UWORD                 ht_PositionNr;
  UBYTE                  ht_SpeedMultiplier;
  UBYTE                  ht_TrackLength;
  UBYTE                  ht_TrackNr;
  UBYTE                  ht_InstrumentNr;
  UBYTE                  ht_SubsongNr;
  UWORD                 ht_PosJump;
  ULONG                 ht_PlayingTime;
  short                  ht_Tempo;
  short                  ht_PosNr;
  short                  ht_StepWaitFrames;
  short                  ht_NoteNr;
  UWORD                 ht_PosJumpNote;
  UBYTE                  ht_GetNewPosition;
  UBYTE                  ht_PatternBreak;
  UBYTE                  ht_SongEndReached;
  UBYTE                  ht_Stereo;
  UWORD                *ht_Subsongs;
  UWORD                 ht_Channels;
  struct hvl_position   *ht_Positions;
  struct hvl_step        ht_Tracks[256][64];
  struct hvl_instrument *ht_Instruments;
  struct hvl_voice       ht_Voices[MAX_CHANNELS];
  int                  ht_defstereo;
  int                  ht_defpanleft;
  int                  ht_defpanright;
  int                  ht_mixgain;
  UBYTE                  ht_Version;
};

void hvl_DecodeFrame( struct hvl_tune *ht, signed char *buf1, signed char *buf2, int bufmod );
void hvl_InitReplayer( void );
int hvl_InitSubsong( struct hvl_tune *ht, ULONG nr );
struct hvl_tune *hvl_LoadTune( STRPTR name, ULONG freq, ULONG defstereo );
void hvl_FreeTune( struct hvl_tune *ht );
struct hvl_tune *hvl_reset( UBYTE *buf, ULONG buflen, ULONG defstereo, ULONG freq);
int hvl_FindLoudest( struct hvl_tune *ht, int maxframes, int usesongend );


