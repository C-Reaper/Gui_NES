#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/PS4_Controller.h"
#include "/home/codeleaded/System/Static/Library/SoundPlayer.h"
#include "/home/codeleaded/System/Static/Library/NES.h"

NES_Bus nes;
PS4_Controller ps4c;
SoundPlayer sb;

double SoundOut(double t){
	while (!NES_Bus_clock(&nes)) {}
	return nes.dAudioSample;
}
void Setup(AlxWindow* w){
	// "/home/codeleaded/Data/NES/SuperMarioBros.nes"
	// "/home/codeleaded/Data/NES/DonkeyKong.nes"
	NES_Bus_Init(&nes,"/home/codeleaded/Data/NES/DonkeyKong.nes");
	NES_Bus_reset(&nes);
	NES_Bus_SetSampleFrequency(&nes,SOUNDPLAYER_SAMPLE_RATE);

	ps4c = PS4_Controller_New("/dev/input/by-id/usb-Sony_Interactive_Entertainment_Wireless_Controller-if03-event-joystick");
	sb = SoundPlayer_New(2,SOUNDPLAYER_BITS_PER_SAMPLE,SOUNDPLAYER_SAMPLE_RATE,SoundOut);
	
	short buffer[1024 * 2];
    memset(buffer,0,sizeof(buffer));
    
	SoundPlayer_Set(&sb,buffer,1024);
	SoundPlayer_Start(&sb);
}
void Update_NoAudio(AlxWindow* w){
	if(Stroke(ALX_KEY_SPACE).PRESSED){
		do{
			C6502_clock(&nes.cpu);
		} 
		while (!C6502_complete(&nes.cpu));
	}
	if(Stroke(ALX_KEY_R).PRESSED){
		C6502_reset(&nes.cpu);
	}
	if(Stroke(ALX_KEY_I).PRESSED){
		C6502_irq(&nes.cpu);
	}
	if(Stroke(ALX_KEY_N).PRESSED){
		C6502_nmi(&nes.cpu);
	}
	if (Stroke(ALX_KEY_SPACE).PRESSED) 	nes.bEmulationRun = !nes.bEmulationRun;

	if (nes.bEmulationRun){
		if (nes.fResidualTime > 0.0f)
			nes.fResidualTime -= w->ElapsedTime;
		else{
			nes.fResidualTime += (1.0f / 60.0f) - w->ElapsedTime;
			do {
				NES_Bus_clock(&nes);
			} while (!nes.ppu.frame_complete);
			nes.ppu.frame_complete = false;
		}
	}else{
		// Emulate code step-by-step
		if (Stroke(ALX_KEY_C).PRESSED){
			do {
				NES_Bus_clock(&nes);
			} while (!C6502_complete(&nes.cpu));
			
			do {
				NES_Bus_clock(&nes);
			} while (!C6502_complete(&nes.cpu));
		}
		// Emulate one whole frame
		if (Stroke(ALX_KEY_F).PRESSED){
			do {
				NES_Bus_clock(&nes);
			} while (!nes.ppu.frame_complete);

			do {
				NES_Bus_clock(&nes);
			} while (!C6502_complete(&nes.cpu));
			
			nes.ppu.frame_complete = false;
		}
	}
}
void Update(AlxWindow* w){
	Clear(BLACK);

	//Update_NoAudio(w);

	PS4_Controller_Update(&ps4c);
	const signed int absx = PS4_Controller_Abs(&ps4c,PS4_CONTROLLER_LX);
	const signed int absy = PS4_Controller_Abs(&ps4c,PS4_CONTROLLER_LY);

	nes.controller[0] = 0x00;
	nes.controller[0] |= Stroke(ALX_KEY_X).DOWN || PS4_Controller_Key(&ps4c,PS4_CONTROLLER_X).DOWN ? 0x80 : 0x00;     // A Button
	nes.controller[0] |= Stroke(ALX_KEY_Z).DOWN || PS4_Controller_Key(&ps4c,PS4_CONTROLLER_O).DOWN ? 0x40 : 0x00;     // B Button
	nes.controller[0] |= Stroke(ALX_KEY_A).DOWN || PS4_Controller_Key(&ps4c,PS4_CONTROLLER_SELECT).DOWN ? 0x20 : 0x00;     // Select
	nes.controller[0] |= Stroke(ALX_KEY_S).DOWN || PS4_Controller_Key(&ps4c,PS4_CONTROLLER_OPTIONS).DOWN ? 0x10 : 0x00;     // Start
	
	nes.controller[0] |= Stroke(ALX_KEY_UP).DOWN || (absy >= 0 && absy < 100) ? 0x08 : 0x00;
	nes.controller[0] |= Stroke(ALX_KEY_DOWN).DOWN || (absy >= 156) ? 0x04 : 0x00;
	nes.controller[0] |= Stroke(ALX_KEY_LEFT).DOWN || (absx >= 0 && absx < 100) ? 0x02 : 0x00;
	nes.controller[0] |= Stroke(ALX_KEY_RIGHT).DOWN || (absx >= 156) ? 0x01 : 0x00;

	// Update_NoAudio(w);
	if (Stroke(ALX_KEY_R).PRESSED || PS4_Controller_Key(&ps4c,PS4_CONTROLLER_HOME).PRESSED)
		NES_Bus_reset(&nes);

	NES_Bus_Render(&nes,WINDOW_STD_ARGS);
}
void Delete(AlxWindow* w){
	SoundPlayer_Free(&sb);
	PS4_Controller_Free(&ps4c);
	NES_Bus_Free(&nes);
}

int main(){
	// 256,240,4,4 | 500,300,4,4
    if(Create("NES_Bus - Emulator",256,240,4,4,Setup,Update,Delete))
        Start();
    return 0;
}