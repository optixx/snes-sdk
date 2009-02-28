/**********************************************************************************
  Snes9x - Portable Super Nintendo Entertainment System (TM) emulator.

  (c) Copyright 1996 - 2002  Gary Henderson (gary.henderson@ntlworld.com) and
                             Jerremy Koot (jkoot@snes9x.com)

  (c) Copyright 2002 - 2004  Matthew Kendora

  (c) Copyright 2002 - 2005  Peter Bortas (peter@bortas.org)

  (c) Copyright 2004 - 2005  Joel Yliluoma (http://iki.fi/bisqwit/)

  (c) Copyright 2001 - 2006  John Weidman (jweidman@slip.net)

  (c) Copyright 2002 - 2006  Brad Jorsch (anomie@users.sourceforge.net),
                             funkyass (funkyass@spam.shaw.ca),
                             Kris Bleakley (codeviolation@hotmail.com),
                             Nach (n-a-c-h@users.sourceforge.net), and
                             zones (kasumitokoduck@yahoo.com)

  BS-X C emulator code
  (c) Copyright 2005 - 2006  Dreamer Nom,
                             zones

  C4 x86 assembler and some C emulation code
  (c) Copyright 2000 - 2003  _Demo_ (_demo_@zsnes.com),
                             Nach,
                             zsKnight (zsknight@zsnes.com)

  C4 C++ code
  (c) Copyright 2003 - 2006  Brad Jorsch,
                             Nach

  DSP-1 emulator code
  (c) Copyright 1998 - 2006  _Demo_,
                             Andreas Naive (andreasnaive@gmail.com)
                             Gary Henderson,
                             Ivar (ivar@snes9x.com),
                             John Weidman,
                             Kris Bleakley,
                             Matthew Kendora,
                             Nach,
                             neviksti (neviksti@hotmail.com)

  DSP-2 emulator code
  (c) Copyright 2003         John Weidman,
                             Kris Bleakley,
                             Lord Nightmare (lord_nightmare@users.sourceforge.net),
                             Matthew Kendora,
                             neviksti


  DSP-3 emulator code
  (c) Copyright 2003 - 2006  John Weidman,
                             Kris Bleakley,
                             Lancer,
                             z80 gaiden

  DSP-4 emulator code
  (c) Copyright 2004 - 2006  Dreamer Nom,
                             John Weidman,
                             Kris Bleakley,
                             Nach,
                             z80 gaiden

  OBC1 emulator code
  (c) Copyright 2001 - 2004  zsKnight,
                             pagefault (pagefault@zsnes.com),
                             Kris Bleakley,
                             Ported from x86 assembler to C by sanmaiwashi

  SPC7110 and RTC C++ emulator code
  (c) Copyright 2002         Matthew Kendora with research by
                             zsKnight,
                             John Weidman,
                             Dark Force

  S-DD1 C emulator code
  (c) Copyright 2003         Brad Jorsch with research by
                             Andreas Naive,
                             John Weidman

  S-RTC C emulator code
  (c) Copyright 2001-2006    byuu,
                             John Weidman

  ST010 C++ emulator code
  (c) Copyright 2003         Feather,
                             John Weidman,
                             Kris Bleakley,
                             Matthew Kendora

  Super FX x86 assembler emulator code
  (c) Copyright 1998 - 2003  _Demo_,
                             pagefault,
                             zsKnight,

  Super FX C emulator code
  (c) Copyright 1997 - 1999  Ivar,
                             Gary Henderson,
                             John Weidman

  Sound DSP emulator code is derived from SNEeSe and OpenSPC:
  (c) Copyright 1998 - 2003  Brad Martin
  (c) Copyright 1998 - 2006  Charles Bilyue'

  SH assembler code partly based on x86 assembler code
  (c) Copyright 2002 - 2004  Marcus Comstedt (marcus@mc.pp.se)

  2xSaI filter
  (c) Copyright 1999 - 2001  Derek Liauw Kie Fa

  HQ2x filter
  (c) Copyright 2003         Maxim Stepin (maxim@hiend3d.com)

  Specific ports contains the works of other authors. See headers in
  individual files.

  Snes9x homepage: http://www.snes9x.com

  Permission to use, copy, modify and/or distribute Snes9x in both binary
  and source form, for non-commercial purposes, is hereby granted without 
  fee, providing that this license information and copyright notice appear 
  with all copies and any derived work.

  This software is provided 'as-is', without any express or implied
  warranty. In no event shall the authors be held liable for any damages
  arising from the use of this software or it's derivatives.

  Snes9x is freeware for PERSONAL USE only. Commercial users should
  seek permission of the copyright holders first. Commercial use includes,
  but is not limited to, charging money for Snes9x or software derived from
  Snes9x, including Snes9x or derivatives in commercial game bundles, and/or
  using Snes9x as a promotion for your commercial product.

  The copyright holders request that bug fixes and improvements to the code
  should be forwarded to them so everyone can benefit from the modifications
  in future versions.

  Super NES and Super Nintendo Entertainment System are trademarks of
  Nintendo Co., Limited and its subsidiary companies.
**********************************************************************************/


/*******************************************************************************
  SNES9X for Mac OS (c) Copyright John Stiles
  
  Snes9x for Mac OS X

  (c) Copyright 2001 - 2006 zones
  (c) Copyright 2002 - 2006 107
  (c) Copyright 2002        PB1400c
  (c) Copyright 2004        Alexander and Sander
  (c) Copyright 2004 - 2005 Steven Seeger
  (c) Copyright 2005        Ryan Vogt
*******************************************************************************/

#include "memmap.h"
#include "soundux.h"

#include <CoreAudio/CoreAudio.h>
#include <AudioUnit/AudioUnit.h>
#include <AudioToolbox/AudioToolbox.h>

#include "mac-prefix.h"
#include "mac-dialog.h"
#include "mac-musicbox.h"
#include "mac-os.h"
#include "mac-snes9x.h"
#include "mac-audio.h"

#define	kAUReverb	(1 << 0)
#define	kAUGraphEQ	(1 << 1)

int	cureffect = kAUReverb;

static void FreeSoundBuffers(void);
static void ConnectAudioUnits(void);
static void DisconnectAudioUnits(void);
static void SaveEffectPresets(void);
static void LoadEffectPresets(Boolean);
static void SetAudioUnitSoundFormatAndVolume(void);
static void ChangeAudioDeviceBufferFrameSize(UInt32, UInt32 *);
static void ReplaceAudioUnitCarbonView(void);
static void ResizeSoundEffectsDialog(ControlRef);
static OSStatus MacAURenderCallBack(void *, AudioUnitRenderActionFlags *, const AudioTimeStamp *, UInt32, UInt32, AudioBufferList *);
static pascal OSStatus SoundEffectsEventHandler(EventHandlerCallRef, EventRef, void *);
static pascal OSStatus SoundEffectsCarbonViewEventHandler(EventHandlerCallRef, EventRef, void *);

static AudioUnit			target;		// the AudioUnit that Snes9x connects to
static AudioUnit			output;		// Default Output
static AudioUnit			converter;	// Sound Format Converter
static AudioUnit			reverb;		// Matrix Reverb
static AudioUnit			grapheq;	// Graphic Equalizer
static UInt32				defaultFrames;
static char					*monoBuffer;
static int					samplesPerSlice;
static int					_buffernos, _blocksize, _samplecount, _maxsamplecount;
static bool8				soundstarted       = false;
static bool8				aueffectAvailable  = true;
static AudioUnitCarbonView	carbonView         = nil;
static EventHandlerUPP		carbonViewEventUPP = nil;
static EventHandlerRef		carbonViewEventRef = nil;
static WindowRef			effectWRef;
static CGSize				effectWSize;

void InitMacSound(void)
{
	OSStatus				err;
	Component				cmp;
	ComponentDescription	desc;
	
	monoBuffer = nil;
	
	target     = nil;
	output     = nil;
	converter  = nil;
	reverb     = nil;
	grapheq    = nil;
	
	so.playback_rate = Settings.SoundPlaybackRate;
	so.stereo        = Settings.Stereo;
	so.sixteen_bit   = Settings.SixteenBitSound;
	so.encoded       = false;

	desc.componentFlags     = 0;
	desc.componentFlagsMask = 0;
	
	// Find and open DefaultOutput
	
	desc.componentType         = kAudioUnitType_Output;
	desc.componentSubType      = kAudioUnitSubType_DefaultOutput;
	desc.componentManufacturer = 0;
                
	cmp = FindNextComponent(nil, &desc);
    if (!cmp)
		QuitWithFatalError(0, "audio 01");
	
	err = OpenAComponent(cmp, &output);
	if (err)
		QuitWithFatalError(err, "audio 02");
		
	// Find and open Apple's AUConverter
	
	desc.componentType         = kAudioUnitType_FormatConverter;
	desc.componentSubType      = kAudioUnitSubType_AUConverter;
	desc.componentManufacturer = kAudioUnitManufacturer_Apple;

	cmp = FindNextComponent(nil, &desc);
	if (!cmp)
		converter = nil;
	else
	{
		err = OpenAComponent(cmp, &converter);
		if (err)
			converter = nil;
	}

	// Find and open Apple's AUMatrixReverb
			
	desc.componentType         = kAudioUnitType_Effect;
	desc.componentSubType      = kAudioUnitSubType_MatrixReverb;
	desc.componentManufacturer = kAudioUnitManufacturer_Apple;

	cmp = FindNextComponent(nil, &desc);
	if (!cmp)
		reverb = nil;
	else
	{
		err = OpenAComponent(cmp, &reverb);
		if (err)
			reverb = nil;
	}
				
	// Find and open Apple's AUGraphicEQ
			
	desc.componentType         = kAudioUnitType_Effect;
	desc.componentSubType      = kAudioUnitSubType_GraphicEQ;
	desc.componentManufacturer = kAudioUnitManufacturer_Apple;

	cmp = FindNextComponent(nil, &desc);
	if (!cmp)
		grapheq = nil;
	else
	{
		err = OpenAComponent(cmp, &grapheq);
		if (err)
			grapheq = nil;
	}

	// Are Effects available ?
		
	aueffectAvailable = (converter && reverb && grapheq) ? true : false;

	// Initialize all AudioUnits (it is necessary to load presets correctly)
	
	err = AudioUnitInitialize(output);
	if (aueffectAvailable)
	{
		err = AudioUnitInitialize(converter);
		err = AudioUnitInitialize(reverb);
		err = AudioUnitInitialize(grapheq);
	}
	
	// Restore presets from plist
	
	LoadEffectPresets(false);
	
	// Connect AudioUnits each other

	ConnectAudioUnits();
}

void DeinitMacSound(void)
{
	OSStatus	err;
	
	// Free monoBuffer
	
	FreeSoundBuffers();
	
	// Disconnect AudioUnits
	
	DisconnectAudioUnits();
	
	// Save presets into plist
	
	SaveEffectPresets();
	
	// Uninitialize all AudioUnits
	
	err = AudioUnitUninitialize(output);
	if (aueffectAvailable)
	{
		err = AudioUnitUninitialize(converter);
		err = AudioUnitUninitialize(reverb);
		err = AudioUnitUninitialize(grapheq);
	}
	
	// Close all AudioUnits

	err = CloseComponent(output);
	if (aueffectAvailable)
	{
		err = CloseComponent(converter);		
		err = CloseComponent(reverb);
		err = CloseComponent(grapheq);
	}
}

static void ConnectAudioUnits(void)
{
	// Note: Using AUGraph may be easier.
	
	OSStatus	err;

	// Decide Snes9x's target AudioUnit
	
	if (!aueffectAvailable)
	{
		aueffect = 0;
		target = output;
	}
	else
	if (aueffect == 0)
		target = output;
	else
		target = converter;

	// Set render callback to target AudioUnit
	
	//err = AudioUnitUninitialize(target);

	AURenderCallbackStruct	v2callback;
	
	v2callback.inputProc       = MacAURenderCallBack;
	v2callback.inputProcRefCon = nil; 
	err = AudioUnitSetProperty(target, kAudioUnitProperty_SetRenderCallback, kAudioUnitScope_Input, 0, &v2callback, sizeof(v2callback));

	//err = AudioUnitInitialize(target);

	// Connect AudioUnits
		
	if (aueffect)
	{
		AudioUnitConnection	connection;
		UInt32				size;
		
		size = sizeof(connection);
		connection.sourceOutputNumber = 0;
		connection.destInputNumber    = 0;
		
		/*
		err = AudioUnitUninitialize(grapheq);
		err = AudioUnitUninitialize(reverb);
		err = AudioUnitUninitialize(output);
		*/
		
		if ((aueffect & kAUReverb) && (aueffect & kAUGraphEQ))
		{
			connection.sourceAudioUnit = target;
			err = AudioUnitSetProperty(grapheq, kAudioUnitProperty_MakeConnection, kAudioUnitScope_Input, 0, &connection, size);

			connection.sourceAudioUnit = grapheq;
			err = AudioUnitSetProperty(reverb,  kAudioUnitProperty_MakeConnection, kAudioUnitScope_Input, 0, &connection, size);

			connection.sourceAudioUnit = reverb;
			err = AudioUnitSetProperty(output,  kAudioUnitProperty_MakeConnection, kAudioUnitScope_Input, 0, &connection, size);

			printf("AudioUnit Connection: Output <- Reverb <- GraphEQ <- Converter <- Snes9x\n");
		}
		else
		if (aueffect & kAUReverb)
		{
			connection.sourceAudioUnit = target;
			err = AudioUnitSetProperty(reverb,  kAudioUnitProperty_MakeConnection, kAudioUnitScope_Input, 0, &connection, size);

			connection.sourceAudioUnit = reverb;
			err = AudioUnitSetProperty(output,  kAudioUnitProperty_MakeConnection, kAudioUnitScope_Input, 0, &connection, size);

			printf("AudioUnit Connection: Output <- Reverb <- Converter <- Snes9x\n");
		}
		else
		if (aueffect & kAUGraphEQ)
		{
			connection.sourceAudioUnit = target;
			err = AudioUnitSetProperty(grapheq, kAudioUnitProperty_MakeConnection, kAudioUnitScope_Input, 0, &connection, size);

			connection.sourceAudioUnit = grapheq;
			err = AudioUnitSetProperty(output,  kAudioUnitProperty_MakeConnection, kAudioUnitScope_Input, 0, &connection, size);

			printf("AudioUnit Connection: Output <- GraphEQ <- Converter <- Snes9x\n");
		}
		
		/*
		err = AudioUnitInitialize(grapheq);
		err = AudioUnitInitialize(reverb);
		err = AudioUnitInitialize(output);
		*/
	}
	else
		printf("AudioUnit Connection: Output <- Snes9x\n");
		
	// Tell Snes9x's format to target AudioUnit
		
	SetAudioUnitSoundFormatAndVolume();

	//printf("Connected AudioUnits\n");
}

static void DisconnectAudioUnits(void)
{
	// Should be called before aueffect has changed	
	// FIXME: Are these ways correct ?
	
	OSStatus	err;
	
	// Remove render callback (?)
	
	//err = AudioUnitUninitialize(target);
	
	AURenderCallbackStruct	v2callback;
	
	v2callback.inputProc       = nil;
	v2callback.inputProcRefCon = nil; 
	err = AudioUnitSetProperty(target, kAudioUnitProperty_SetRenderCallback, kAudioUnitScope_Input, 0, &v2callback, sizeof(v2callback));

	//err = AudioUnitInitialize(target);
	
	// Disconnect AudioUnits (?)
	
	if (aueffect)
	{
		AudioStreamBasicDescription	format;
		AudioUnitConnection			connection;
		UInt32						size;
		
		size = sizeof(connection);
		connection.sourceOutputNumber = 0;
		connection.destInputNumber    = 0;
		connection.sourceAudioUnit    = nil;
		
		/*
		err = AudioUnitUninitialize(grapheq);
		err = AudioUnitUninitialize(reverb);
		err = AudioUnitUninitialize(output);
		*/
		
		if ((aueffect & kAUReverb) && (aueffect & kAUGraphEQ))
		{
			err = AudioUnitSetProperty(grapheq, kAudioUnitProperty_MakeConnection, kAudioUnitScope_Input, 0, &connection, size);
			err = AudioUnitSetProperty(reverb,  kAudioUnitProperty_MakeConnection, kAudioUnitScope_Input, 0, &connection, size);
			err = AudioUnitSetProperty(output,  kAudioUnitProperty_MakeConnection, kAudioUnitScope_Input, 0, &connection, size);
		}
		else
		if (aueffect & kAUReverb)
		{
			err = AudioUnitSetProperty(reverb,  kAudioUnitProperty_MakeConnection, kAudioUnitScope_Input, 0, &connection, size);
			err = AudioUnitSetProperty(output,  kAudioUnitProperty_MakeConnection, kAudioUnitScope_Input, 0, &connection, size);
		}
		else
		if (aueffect & kAUGraphEQ)
		{
			err = AudioUnitSetProperty(grapheq, kAudioUnitProperty_MakeConnection, kAudioUnitScope_Input, 0, &connection, size);
			err = AudioUnitSetProperty(output,  kAudioUnitProperty_MakeConnection, kAudioUnitScope_Input, 0, &connection, size);
		}
		
		// Get back default input format (unnecessary ?)
		
		size = sizeof(format);
		err = AudioUnitGetProperty(output, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output, 0, &format, &size);
		err = AudioUnitSetProperty(output, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input,  0, &format,  size);
		
		/*
		err = AudioUnitInitialize(grapheq);
		err = AudioUnitInitialize(reverb);
		err = AudioUnitInitialize(output);
		*/
	}
	
	target = output;

	//printf("Disconnected AudioUnits\n");
}

void MacStartSound(void)
{
	OSStatus	err;
	
	if (macQTRecord)
		return;
	
	if (!soundstarted)
	{
		ChangeAudioDeviceBufferFrameSize(samplesPerSlice, &defaultFrames);

		err = AudioOutputUnitStart(output);
		soundstarted = true;
		
		printf("AudioUnit output started.\n");
	}
}

void MacStopSound(void)
{
	OSStatus	err;

	if (macQTRecord)
		return;

	if (soundstarted)
	{	
		err = AudioOutputUnitStop(output);
		soundstarted = false;
		
		ChangeAudioDeviceBufferFrameSize(defaultFrames, nil);
		
		printf("AudioUnit output stopped.\n");
	}
}	

static void FreeSoundBuffers(void)
{
	if (monoBuffer)
	{
		free(monoBuffer);
		monoBuffer = nil;
	}
}

static void SetAudioUnitSoundFormatAndVolume(void)
{
	OSStatus					err;	
	AudioStreamBasicDescription	format;
	UInt32						size;
	
	bzero(&format, sizeof(format));
	
	// Always stereo even if Snes9x outputs mono

	format.mSampleRate	     = (Float64) so.playback_rate;
	format.mFormatID	     = kAudioFormatLinearPCM;
#ifdef __BIG_ENDIAN__
	format.mFormatFlags	     = kLinearPCMFormatFlagIsBigEndian | (so.sixteen_bit ? kLinearPCMFormatFlagIsSignedInteger : 0);
#else
	format.mFormatFlags      = (so.sixteen_bit ? kLinearPCMFormatFlagIsSignedInteger : 0);
#endif
	format.mBytesPerPacket   = 2 * (so.sixteen_bit ? 2 : 1);
	format.mFramesPerPacket  = 1;
	format.mBytesPerFrame    = 2 * (so.sixteen_bit ? 2 : 1);
	format.mChannelsPerFrame = 2;
	format.mBitsPerChannel   = so.sixteen_bit ? 16 : 8;
	
	/*
	if (target == output)
		printf("target == output\n");
	else
		printf("target == converter\n");
	*/
	
	/*
	err = AudioUnitUninitialize(target);
	if (aueffect)
		err = AudioUnitUninitialize(output);
	*/

	// Tell Snes9x's format to target AudioUnit

	size = sizeof(format);
	err = AudioUnitSetProperty(target, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, 0, &format, size);
	
	// Set volume of output AudioUnit

	err = AudioUnitSetParameter(output, kAudioUnitParameterUnit_LinearGain, kAudioUnitScope_Output, 0, (float) macSoundVolume / 100.0, 0);

	/*
	err = AudioUnitInitialize(target);
	if (aueffect)
		err = AudioUnitInitialize(output);
	*/
		
	//printf("Set format and volume \n");
}

static void ChangeAudioDeviceBufferFrameSize(UInt32 newvalue, UInt32 *old)
{
	OSStatus		err;
	AudioDeviceID	device;
	AudioTimeStamp	ts;
	UInt32			bufferSizeInFrames, value, size;
	
	if (macSoundInterval == 0)
	{
		printf("Using default output buffer size.\n");
		return;
	}
	
	// Get current AudioDevice
		
	size = sizeof(device);
	err = AudioUnitGetProperty(output, kAudioOutputUnitProperty_CurrentDevice, kAudioUnitScope_Global, 0, &device, &size);
	if (err)
		QuitWithFatalError(err, "audio 03");
	
	if (old)
	{
		// Remember default value
		
		size = sizeof(value);
		err = AudioDeviceGetProperty(device, 0, false, kAudioDevicePropertyBufferFrameSize, &size, &value);
		if (err)
			QuitWithFatalError(err, "audio 04");
		*old = value;
	}

	bufferSizeInFrames = newvalue;
	
	// Set new bufferSizeInFrames to AudioDevice

	ts.mFlags = 0;
	size = sizeof(bufferSizeInFrames);
	err = AudioDeviceSetProperty(device, &ts, 0, false, kAudioDevicePropertyBufferFrameSize,  size, &bufferSizeInFrames);
	if (err)
		QuitWithFatalError(err, "audio 05");

	// Confirm it

	err = AudioDeviceGetProperty(device,      0, false, kAudioDevicePropertyBufferFrameSize, &size, &bufferSizeInFrames);
	if (err)
		QuitWithFatalError(err, "audio 06");

	err = AudioUnitUninitialize(output);	// necessary ?
	
	// Tell the new bufferSizeInFrames to output AudioUnit
	// V1 DefaultOutput doesn't support MaximumFramesPerSlice ? Ignore error

	size = sizeof(bufferSizeInFrames);
	err = AudioUnitSetProperty(output, kAudioUnitProperty_MaximumFramesPerSlice, kAudioUnitScope_Global, 0, &bufferSizeInFrames,  size);

	err = AudioUnitInitialize(output);		// necessary ?
}

static OSStatus MacAURenderCallBack(void *inRefCon, AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber, UInt32 inNumFrames, AudioBufferList *ioData)
{
	#pragma unused (inRefCon, inTimeStamp, inBusNumber, inNumFrames)
	
	if (so.mute_sound || !Settings.APUEnabled)
	{
		bzero(ioData->mBuffers[0].mData, ioData->mBuffers[0].mDataByteSize);
		*ioActionFlags |= kAudioUnitRenderAction_OutputIsSilence;
	}
	else
	if (so.stereo)
	{
		unsigned int	samples;
		
		samples = ioData->mBuffers[0].mDataByteSize;
		if (so.sixteen_bit)
			samples >>= 1;

		S9xMixSamples((uint8 *) ioData->mBuffers[0].mData, samples);
	}
	else	// Manually map L to R
	{
		unsigned int	monosize, i;
		
		monosize = so.buffer_size;
		if (monosize > (ioData->mBuffers[0].mDataByteSize >> 1))
			monosize = (ioData->mBuffers[0].mDataByteSize >> 1);
		if (so.sixteen_bit)
			monosize >>= 1;
		
		S9xMixSamples((uint8 *) monoBuffer, monosize);

		if (so.sixteen_bit)
		{
			for (i = 0; i < monosize; i++)
			{
				((short *) ioData->mBuffers[0].mData)[i * 2    ] = ((short *) monoBuffer)[i];
				((short *) ioData->mBuffers[0].mData)[i * 2 + 1] = ((short *) monoBuffer)[i];
			}
		}
		else		
		{
			for (i = 0; i < monosize; i++)
			{
				((char  *) ioData->mBuffers[0].mData)[i * 2    ] = monoBuffer[i];
				((char  *) ioData->mBuffers[0].mData)[i * 2 + 1] = monoBuffer[i];
			}
		}
	}

	#ifdef MK_APU
	if (SoundData.sample_cycles < 0)
		SoundData.sample_cycles = 0;
	#endif

	return noErr;
}

bool8 S9xOpenSoundDevice(int mode, bool8 stereo, int buffer_size)
{
	#pragma unused (mode, stereo, buffer_size)

	int	bytesPerBlock;
	
	so.mute_sound = true;

	FreeSoundBuffers();

	so.playback_rate = Settings.SoundPlaybackRate;
	so.stereo        = Settings.Stereo;
	so.sixteen_bit   = Settings.SixteenBitSound;
	so.encoded       = false;
	
	/*************************/
	
	// v43 -> v43.1
	// Thanks to '2' testers !
	
	// Base value
	samplesPerSlice = (int) ((float) so.playback_rate * ((float) macSoundInterval / 1000.0));

	//printf("Calculated samplesPerSlice:%d\n", samplesPerSlice);
	
	// A : align 16 samples ?
	//samplesPerSlice = (samplesPerSlice + 0xF) & 0xFFFFFFF0;
	
	// B : align power of 2 ?
	int	temp = 1;
	while (temp < samplesPerSlice)
		temp <<= 1;
	samplesPerSlice = temp;
	
	//printf("Aligned samplesPerSlice:%d\n", samplesPerSlice);
	
	/*************************/
	
	_maxsamplecount = SOUND_BUFFER_SIZE;
	if (so.sixteen_bit)
		_maxsamplecount >>= 1;
	if (so.stereo)
		_maxsamplecount >>= 1;
		
	if (samplesPerSlice > _maxsamplecount)
		samplesPerSlice = _maxsamplecount;

	_blocksize = samplesPerSlice;
	if (so.sixteen_bit)
		_blocksize <<= 1;
	if (so.stereo)
		_blocksize <<= 1;
	
	bytesPerBlock = 16;
	while (bytesPerBlock < _blocksize)
		bytesPerBlock <<= 1;

	so.buffer_size = _blocksize;

	_buffernos = 1;

	_samplecount = so.sixteen_bit ? (_blocksize >> 1) : _blocksize;

	so.samples_mixed_so_far = 0;

	if (so.stereo == false)
	{
		monoBuffer = (char *) calloc(bytesPerBlock, 1);
		if (monoBuffer == nil)
			QuitWithFatalError(0, "audio 07");
	}
	
	SetAudioUnitSoundFormatAndVolume();

	macSoundPitchMaster = macSoundPitch;

	S9xSetPlaybackRate(so.playback_rate);

	so.mute_sound = false;
	
	return true;
}

extern "C" void S9xGenerateSound(void)
{
	// If rapid sound generation is needed, I'll implement it later.
	
	return;
}

static void ReplaceAudioUnitCarbonView(void)
{
	OSStatus				err;
	AudioUnit				editau;
	Component				cmp;
	ComponentDescription	desc;
	ControlRef				viewPane, root, ctl;
	ControlID				cid;
	Float32Point			location, size;
	Rect					rct;
	UInt32					psize;
	
	// Release previous AudioUnitCarbonView
	
	if (carbonView)
	{
		err = RemoveEventHandler(carbonViewEventRef);
		DisposeEventHandlerUPP(carbonViewEventUPP);
		carbonViewEventRef = nil;
		carbonViewEventUPP = nil;
		
		CloseComponent(carbonView);
		carbonView = nil;
	}

	// Decide AudioUnit to configure

	switch (cureffect)
	{
		case kAUGraphEQ:
			editau = grapheq;
			break;
			
		case kAUReverb:
		default:
			editau = reverb;		
	}

	desc.componentType         = kAudioUnitCarbonViewComponentType;
	desc.componentSubType      = kAUCarbonViewSubType_Generic;
	desc.componentManufacturer = kAudioUnitManufacturer_Apple;
	desc.componentFlags        = 0;
	desc.componentFlagsMask    = 0;
	
	// Find the first UI of editau

	err = AudioUnitGetPropertyInfo(editau, kAudioUnitProperty_GetUIComponentList, kAudioUnitScope_Global, 0, &psize, nil);
	if (err == noErr)
	{
		ComponentDescription	*editors;
		int						nEditors;
		
		nEditors = psize / sizeof(ComponentDescription);
		
		editors = new ComponentDescription [nEditors];
		
		err = AudioUnitGetProperty(editau, kAudioUnitProperty_GetUIComponentList, kAudioUnitScope_Global, 0, editors, &psize);
		if (err == noErr)
			desc = editors[0];

		delete [] editors;
	}

	// Open AudioUnitCarbonView
	
	cmp = FindNextComponent(nil, &desc);
	if (cmp)
	{
		err = OpenAComponent(cmp, &carbonView);
		if (err == noErr)
		{
			// Create AudioUnitCarbonView, resize and move window
						
			GetRootControl(effectWRef, &root);
			GetWindowBounds(effectWRef, kWindowContentRgn, &rct);
			
			location.x = 20;
			location.y = 94;
			size.x     = rct.right  - rct.left;
			size.y     = rct.bottom - rct.top;
			
			err = AudioUnitCarbonViewCreate(carbonView, editau, effectWRef, root, &location, &size, &viewPane);
			
			EventTypeSpec	event[] = { { kEventClassControl, kEventControlBoundsChanged } };
			
			carbonViewEventUPP = NewEventHandlerUPP(SoundEffectsCarbonViewEventHandler);
			err = InstallControlEventHandler(viewPane, carbonViewEventUPP, GetEventTypeCount(event), event, (void *) effectWRef, &carbonViewEventRef);

			ResizeSoundEffectsDialog(viewPane);
		}
		else
			carbonView = nil;
	}
	else
		carbonView = nil;

	// Set CheckBox

	cid.id = 0;
	cid.signature = 'Enab';
	GetControlByID(effectWRef, &cid, &ctl);
	SetControl32BitValue(ctl, (aueffect & cureffect) ? 1 : 0);
}

static void ResizeSoundEffectsDialog(ControlRef view)
{
	OSStatus	err;
	ControlRef	ctl;
	ControlID	cid;
	Rect		rv;
	int			w, h;
	
	cid.id = 0;
	cid.signature = 'Enab';
	GetControlByID(effectWRef, &cid, &ctl);
	err = SetControlVisibility(ctl,  false, true);
	err = SetControlVisibility(view, false, true);
	
	GetControlBounds(view, &rv);
	w = (rv.right - rv.left + 30 > (int) effectWSize.width) ? (rv.right - rv.left + 30) : (int) effectWSize.width;
	h = rv.bottom - rv.top + 120;
#ifdef MAC_PANTHER_JAGUAR_SUPPORT
	if (systemVersion < 0x1040)
		h += 16;
#endif
	
	GetWindowBounds(effectWRef, kWindowStructureRgn, &rv);
	rv.right  = rv.left + w;
	rv.bottom = rv.top  + h;
	err = TransitionWindow(effectWRef, kWindowSlideTransitionEffect, kWindowResizeTransitionAction, &rv);
	
	err = SetControlVisibility(ctl,  true, true);
	err = SetControlVisibility(view, true, true);
	
#ifdef MAC_PANTHER_JAGUAR_SUPPORT
	if (systemVersion < 0x1040)
	{
		Rect	rct;
		
		GetWindowBounds(effectWRef, kWindowContentRgn, &rv);
		
		cid.signature = 'SfUI';
		GetControlByID(effectWRef, &cid, &ctl);
		GetControlBounds(ctl, &rct);
		SizeControl(ctl, rv.right - rv.left, rct.bottom - rct.top);
		
		cid.signature = 'LINE';
		GetControlByID(effectWRef, &cid, &ctl);
		GetControlBounds(ctl, &rct);
		SizeControl(ctl, rv.right - rv.left - 24, rct.bottom - rct.top);
		
		rct.top    = 0;
		rct.left   = 0;
		rct.bottom = rv.bottom - rv.top;
		rct.right  = rv.right  - rv.left;
		err = InvalWindowRect(effectWRef, &rct);
	}
#endif
}

void ConfigureSoundEffects(void)
{
	OSStatus	err;
	IBNibRef	nibRef;
	
	err = CreateNibReference(kMacS9XCFString, &nibRef);
	if (err == noErr)
	{
		WindowRef	uiparts;
		
		err = CreateWindowFromNib(nibRef, CFSTR("SoundEffect"), &uiparts);
		if (err == noErr)
		{
			EventHandlerUPP		eventUPP;
			EventHandlerRef		eventHandler;
			EventTypeSpec		event[] = { { kEventClassWindow, kEventWindowClose }, { kEventClassCommand, kEventCommandProcess } };
			MenuRef				menu;
			ControlRef			ctl, userpane, contentview;
			ControlID			cid;
			CFStringRef			str;
			Rect				rct;
			WindowAttributes	metal = 0;
			
			cid.id = 0;
			cid.signature = 'SfUI';
			GetControlByID(uiparts, &cid, &userpane);
			GetWindowBounds(uiparts, kWindowContentRgn, &rct);
			
			if (systemVersion >= 0x1040)	// AUs support compositing
			{
				HIRect	frame;
				
				str = CFCopyLocalizedString(CFSTR("CreateMetalDlg"), "NO");
				if (str)
				{
					if (CFStringCompare(str, CFSTR("YES"), 0) == kCFCompareEqualTo)
						metal = kWindowMetalAttribute;
					
					CFRelease(str);
				}

				frame = CGRectMake(0.0, 0.0, (float) (rct.right - rct.left), (float) (rct.bottom - rct.top));
				err = CreateNewWindow(kDocumentWindowClass, kWindowCloseBoxAttribute | kWindowCollapseBoxAttribute | kWindowStandardHandlerAttribute | kWindowCompositingAttribute | metal, &rct, &effectWRef);
				err = HIViewFindByID(HIViewGetRoot(effectWRef), kHIViewWindowContentID, &contentview);
				err = HIViewAddSubview(contentview, userpane);
				err = HIViewSetFrame(userpane, &frame);
			}
		#ifdef MAC_PANTHER_JAGUAR_SUPPORT
			else
			{
				err = CreateNewWindow(kDocumentWindowClass, kWindowCloseBoxAttribute | kWindowCollapseBoxAttribute | kWindowStandardHandlerAttribute, &rct, &effectWRef);
				err = CreateRootControl(effectWRef, &contentview);
				err = EmbedControl(userpane, contentview);
				MoveControl(userpane, 0, 0);
			}
		#endif
			
			ReleaseWindow(uiparts);
			
			if (!metal)
				err = SetThemeWindowBackground(effectWRef, kThemeBrushDialogBackgroundActive, false);
			
			str = CFCopyLocalizedString(CFSTR("SoundEffectDlg"), "SoundEffect");
			if (str)
			{
				err = SetWindowTitleWithCFString(effectWRef, str);
				CFRelease(str);
			}
			
			if (systemVersion >= 0x1040)	// AUs support compositing
			{
				HILayoutInfo	layoutinfo;
				HIViewRef		separator;
				
				cid.signature = 'LINE';
				err = HIViewFindByID(userpane, cid, &separator);
				
				layoutinfo.version = kHILayoutInfoVersionZero;
				err = HIViewGetLayoutInfo(userpane, &layoutinfo);
				
				layoutinfo.binding.top.toView    = contentview;
				layoutinfo.binding.top.kind      = kHILayoutBindNone;
				layoutinfo.binding.bottom.toView = contentview;
				layoutinfo.binding.bottom.kind   = kHILayoutBindNone;
				layoutinfo.binding.left.toView   = contentview;
				layoutinfo.binding.left.kind     = kHILayoutBindLeft;
				layoutinfo.binding.right.toView  = contentview;
				layoutinfo.binding.right.kind    = kHILayoutBindRight;
				err = HIViewSetLayoutInfo(userpane, &layoutinfo);
				
				layoutinfo.version = kHILayoutInfoVersionZero;
				err = HIViewGetLayoutInfo(separator, &layoutinfo);

				layoutinfo.binding.top.toView    = userpane;
				layoutinfo.binding.top.kind      = kHILayoutBindNone;
				layoutinfo.binding.bottom.toView = userpane;
				layoutinfo.binding.bottom.kind   = kHILayoutBindNone;
				layoutinfo.binding.left.toView   = userpane;
				layoutinfo.binding.left.kind     = kHILayoutBindLeft;
				layoutinfo.binding.right.toView  = userpane;
				layoutinfo.binding.right.kind    = kHILayoutBindRight;
				err = HIViewSetLayoutInfo(separator, &layoutinfo);
			}
			
			eventUPP = NewEventHandlerUPP(SoundEffectsEventHandler);
			err = InstallWindowEventHandler(effectWRef, eventUPP, GetEventTypeCount(event), event, (void *) effectWRef, &eventHandler);
			
			GetWindowBounds(effectWRef, kWindowContentRgn, &rct);
			effectWSize.width  = (float) (rct.right  - rct.left);
			effectWSize.height = (float) (rct.bottom - rct.top );
			
			carbonView = nil;
			ReplaceAudioUnitCarbonView();
			
			LoadEffectPresets(true);
			
			cid.signature = 'Epop';
			GetControlByID(effectWRef, &cid, &ctl);
			menu = GetControlPopupMenuHandle(ctl);
			for (int i = 1; i <= CountMenuItems(menu); i++)
				CheckMenuItem(menu, i, false);
			switch (cureffect)
			{
				case kAUReverb:
					CheckMenuItem(menu, 1, true);
					SetControl32BitValue(ctl, 1);
					break;
					
				case kAUGraphEQ:
					CheckMenuItem(menu, 2, true);
					SetControl32BitValue(ctl, 2);
			}
			
			MoveWindowPosition(effectWRef, kWindowSoundEffect, false);
			ShowWindow(effectWRef);
			err = RunAppModalLoopForWindow(effectWRef);
			HideWindow(effectWRef);
			SaveWindowPosition(effectWRef, kWindowSoundEffect);
			
			SaveEffectPresets();
			
			if (carbonView)
			{
				err = RemoveEventHandler(carbonViewEventRef);
				DisposeEventHandlerUPP(carbonViewEventUPP);
				carbonViewEventRef = nil;
				carbonViewEventUPP = nil;
				
				CloseComponent(carbonView);
				carbonView = nil;
			}
			
			err = RemoveEventHandler(eventHandler);
			DisposeEventHandlerUPP(eventUPP);
			
			ReleaseWindow(effectWRef);
		}
		
		DisposeNibReference(nibRef);
	}
}

static pascal OSStatus SoundEffectsCarbonViewEventHandler(EventHandlerCallRef inHandlerRef, EventRef inEvent, void *inUserData)
{
	#pragma unused (inHandlerRef)
	
	OSStatus	err, result = eventNotHandledErr;
	ControlRef	ctl;
	Rect		bounds;

	switch (GetEventClass(inEvent))
	{
		case kEventClassControl:
			switch (GetEventKind(inEvent))
			{
				case kEventControlBoundsChanged:
					err = GetEventParameter(inEvent, kEventParamDirectObject, typeControlRef, nil, sizeof(ControlRef), nil, &ctl);
					if (err == noErr)
					{
						err = GetEventParameter(inEvent, kEventParamCurrentBounds, typeQDRectangle, nil, sizeof(Rect), nil, &bounds);
						if (err == noErr)
						{
							if ((bounds.right - bounds.left > 0) && (bounds.bottom - bounds.top > 0))
								ResizeSoundEffectsDialog(ctl);
						}
					}
					
					result = noErr;
			}
	}
	
	return result;
}

static pascal OSStatus SoundEffectsEventHandler(EventHandlerCallRef inHandlerRef, EventRef inEvent, void *inUserData)
{
	#pragma unused (inHandlerRef)
	
	OSStatus	err, result = eventNotHandledErr;
	WindowRef	tWindowRef = (WindowRef) inUserData;

	switch (GetEventClass(inEvent))
	{
		case kEventClassWindow:
			switch (GetEventKind(inEvent))
			{
				case kEventWindowClose:
					QuitAppModalLoopForWindow(tWindowRef);
					result = noErr;
			}
			
			break;

		case kEventClassCommand:
			switch (GetEventKind(inEvent))
			{
				case kEventCommandProcess:
					HICommand	tHICommand;

					err = GetEventParameter(inEvent, kEventParamDirectObject, typeHICommand, nil, sizeof(HICommand), nil, &tHICommand);
					if (err == noErr)
					{
						switch (tHICommand.commandID)
						{
							case 'Enab':
								mboxPause = true;
								err = AudioOutputUnitStop(output);
								DisconnectAudioUnits();								
								aueffect ^= cureffect;								
								ConnectAudioUnits();
								err = AudioOutputUnitStart(output);
								mboxPause = false;

								result = noErr;
								break;
								
							case 'Revb':
								cureffect = kAUReverb;
								ReplaceAudioUnitCarbonView();

								break;

							case 'GrEQ':
								cureffect = kAUGraphEQ;
								ReplaceAudioUnitCarbonView();
						}
					}
			}
	}
	
	return result;
}

static void SaveEffectPresets(void)
{
	if (aueffectAvailable)
	{
		OSStatus			err;
		AUPreset			preset;
		CFPropertyListRef	classData;	
		UInt32				size;
		
		preset.presetNumber = -1;	// User Preset
		preset.presetName   = CFSTR("SNES9X Preset");
	
		err = AudioUnitSetProperty(reverb,  kAudioUnitProperty_CurrentPreset, kAudioUnitScope_Global, 0, &preset, sizeof(preset));
		if (err == noErr)
		{
			size = sizeof(classData);
			err = AudioUnitGetProperty(reverb,  kAudioUnitProperty_ClassInfo, kAudioUnitScope_Global, 0, &classData, &size);
			if (err == noErr)
			{
				CFPreferencesSetAppValue(CFSTR("Effect_Preset_Reverb"),  classData, kCFPreferencesCurrentApplication);
				CFRelease(classData);
			}
		}
		
		err = AudioUnitSetProperty(grapheq, kAudioUnitProperty_CurrentPreset, kAudioUnitScope_Global, 0, &preset, sizeof(preset));
		if (err == noErr)
		{
			size = sizeof(classData);
			err = AudioUnitGetProperty(grapheq, kAudioUnitProperty_ClassInfo, kAudioUnitScope_Global, 0, &classData, &size);
			if (err == noErr)
			{
				CFPreferencesSetAppValue(CFSTR("Effect_Preset_GraphEQ"), classData, kCFPreferencesCurrentApplication);
				CFRelease(classData);
			}
		}
		
		CFPreferencesAppSynchronize(kCFPreferencesCurrentApplication);
	}
}

static void LoadEffectPresets(Boolean notify)
{
	if (aueffectAvailable)
	{
		OSStatus				err;
		AudioUnitParameter		changedUnit;
		ComponentDescription	cd;
		CFPropertyListRef		classData;
		CFDictionaryRef 		dict;	
		CFNumberRef				cfnum;
		
		classData = CFPreferencesCopyAppValue(CFSTR("Effect_Preset_Reverb"),  kCFPreferencesCurrentApplication);
		if (classData)
		{
			dict = (CFDictionaryRef) classData;
			
			cfnum = reinterpret_cast <CFNumberRef> (CFDictionaryGetValue(dict, CFSTR("type")));
			CFNumberGetValue(cfnum, kCFNumberSInt32Type, &cd.componentType);	
			cfnum = reinterpret_cast <CFNumberRef> (CFDictionaryGetValue(dict, CFSTR("subtype")));
			CFNumberGetValue(cfnum, kCFNumberSInt32Type, &cd.componentSubType);
			cfnum = reinterpret_cast <CFNumberRef> (CFDictionaryGetValue(dict, CFSTR("manufacturer")));
			CFNumberGetValue(cfnum, kCFNumberSInt32Type, &cd.componentManufacturer);
	
			if ((cd.componentType         == kAudioUnitType_Effect         ) &&
				(cd.componentSubType      == kAudioUnitSubType_MatrixReverb) &&
				(cd.componentManufacturer == kAudioUnitManufacturer_Apple  ))
				err = AudioUnitSetProperty(reverb,  kAudioUnitProperty_ClassInfo, kAudioUnitScope_Global, 0, &classData, sizeof(classData));

			if (notify)
			{
				changedUnit.mAudioUnit   = reverb;
				changedUnit.mParameterID = kAUParameterListener_AnyParameter;
				err = AUParameterListenerNotify(nil, nil, &changedUnit);
			}
				
			CFRelease(classData);
		}
		
		classData = CFPreferencesCopyAppValue(CFSTR("Effect_Preset_GraphEQ"), kCFPreferencesCurrentApplication);
		if (classData)
		{
			dict = (CFDictionaryRef) classData;
			
			cfnum = reinterpret_cast <CFNumberRef> (CFDictionaryGetValue(dict, CFSTR("type")));
			CFNumberGetValue(cfnum, kCFNumberSInt32Type, &cd.componentType);	
			cfnum = reinterpret_cast <CFNumberRef> (CFDictionaryGetValue(dict, CFSTR("subtype")));
			CFNumberGetValue(cfnum, kCFNumberSInt32Type, &cd.componentSubType);
			cfnum = reinterpret_cast <CFNumberRef> (CFDictionaryGetValue(dict, CFSTR("manufacturer")));
			CFNumberGetValue(cfnum, kCFNumberSInt32Type, &cd.componentManufacturer);
	
			if ((cd.componentType         == kAudioUnitType_Effect       ) &&
				(cd.componentSubType      == kAudioUnitSubType_GraphicEQ ) &&
				(cd.componentManufacturer == kAudioUnitManufacturer_Apple))
				err = AudioUnitSetProperty(grapheq, kAudioUnitProperty_ClassInfo, kAudioUnitScope_Global, 0, &classData, sizeof(classData));
				
			if (notify)
			{
				changedUnit.mAudioUnit   = grapheq;
				changedUnit.mParameterID = kAUParameterListener_AnyParameter;
				err = AUParameterListenerNotify(nil, nil, &changedUnit);
			}

			CFRelease(classData);
		}
	}
}
