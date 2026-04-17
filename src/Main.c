#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/NES.h"

NES_Bus nes;

void Setup(AlxWindow* w){
	nes = NES_Bus_New("/home/codeleaded/Data/NES_Bus/SuperMarioBros.NES_Bus");
	C6502_ConnectNES_Bus(&nes.cpu,&nes);
	C6502_reset(&nes.cpu);
}
void Update(AlxWindow* w){
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

	Clear(BLACK);
	
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