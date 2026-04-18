#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/NES.h"

NES_Bus nes;

void Setup(AlxWindow* w){
	// "/home/codeleaded/Data/NES/SuperMarioBros.nes"
	// "/home/codeleaded/Data/NES/DonkeyKong.nes"
	NES_Bus_Init(&nes,"/home/codeleaded/Data/NES/SuperMarioBros.nes");
	C6502_reset(&nes.cpu);
}
void Update(AlxWindow* w){
	Clear(BLACK);

	nes.controller[0] = 0x00;
	nes.controller[0] |= Stroke(ALX_KEY_X).DOWN ? 0x80 : 0x00;     // A Button
	nes.controller[0] |= Stroke(ALX_KEY_Z).DOWN ? 0x40 : 0x00;     // B Button
	nes.controller[0] |= Stroke(ALX_KEY_A).DOWN ? 0x20 : 0x00;     // Select
	nes.controller[0] |= Stroke(ALX_KEY_S).DOWN ? 0x10 : 0x00;     // Start
	nes.controller[0] |= Stroke(ALX_KEY_UP).DOWN ? 0x08 : 0x00;
	nes.controller[0] |= Stroke(ALX_KEY_DOWN).DOWN ? 0x04 : 0x00;
	nes.controller[0] |= Stroke(ALX_KEY_LEFT).DOWN ? 0x02 : 0x00;
	nes.controller[0] |= Stroke(ALX_KEY_RIGHT).DOWN ? 0x01 : 0x00;

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
	if (Stroke(ALX_KEY_R).PRESSED) 		NES_Bus_reset(&nes);

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
	
	NES_Bus_Render(&nes,WINDOW_STD_ARGS);
}
void Delete(AlxWindow* w){
	NES_Bus_Free(&nes);
}

int main(){
    if(Create("NES_Bus - Emulator",680,480,2,2,Setup,Update,Delete))
        Start();
    return 0;
}