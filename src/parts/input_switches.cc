/* ########################################################################

   PICSimLab - Programmable IC Simulator Laboratory

   ########################################################################

   Copyright (c) : 2010-2023  Luis Claudio Gambôa Lopes <lcgamboa@yahoo.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   For e-mail suggestions :  lcgamboa@yahoo.com
   ######################################################################## */

#include "input_switches.h"
#include "../lib/oscilloscope.h"
#include "../lib/picsimlab.h"
#include "../lib/spareparts.h"

/* outputs */
enum { O_P1, O_P2, O_P3, O_P4, O_P5, O_P6, O_P7, O_P8, O_S1, O_S2, O_S3, O_S4, O_S5, O_S6, O_S7, O_S8 };

/* inputs */
enum { I_S1, I_S2, I_S3, I_S4, I_S5, I_S6, I_S7, I_S8 };

enum { MODE_NORMAL, MODE_IDEAL };

static PCWProp pcwprop[14] = {{PCW_LABEL, "1 -VCC,+5V"}, {PCW_COMBO, "2 -Out 1"},
                              {PCW_COMBO, "3 -Out 2"},   {PCW_COMBO, "4 -Out 3"},
                              {PCW_COMBO, "5 -Out 4"},   {PCW_COMBO, "6 -Out 5"},
                              {PCW_COMBO, "7 -Out 6"},   {PCW_COMBO, "8 -Out 7"},
                              {PCW_COMBO, "9 -Out 8"},   {PCW_LABEL, "10-GND ,GND"},
                              {PCW_COMBO, "Active"},     {PCW_COMBO, "Mode"},
                              {PCW_SPIN, "Size"},        {PCW_END, ""}};

cpart_switches::cpart_switches(const unsigned x, const unsigned y, const char* name, const char* type)
    : part(x, y, name, type), font(8, lxFONTFAMILY_TELETYPE, lxFONTSTYLE_NORMAL, lxFONTWEIGHT_BOLD) {
    active = 1;
    Size = 0;
    Bitmap = NULL;

    ReadMaps();
    OWidth = Width;
    OHeight = Height;

    ChangeSize(8);

    output_pins[0] = 0;
    output_pins[1] = 0;
    output_pins[2] = 0;
    output_pins[3] = 0;
    output_pins[4] = 0;
    output_pins[5] = 0;
    output_pins[6] = 0;
    output_pins[7] = 0;

    output_value[0] = 0;
    output_value[1] = 0;
    output_value[2] = 0;
    output_value[3] = 0;
    output_value[4] = 0;
    output_value[5] = 0;
    output_value[6] = 0;
    output_value[7] = 0;

    mode = MODE_NORMAL;

    SWBounce_init(&bounce, 8);

    SetPCWProperties(pcwprop);

    PinCount = 8;
    Pins = output_pins;
}

void cpart_switches::RegisterRemoteControl(void) {
    input_ids[I_S1]->status = &output_value[0];
    input_ids[I_S1]->update = &output_ids[O_S1]->update;
    input_ids[I_S2]->status = &output_value[1];
    input_ids[I_S2]->update = &output_ids[O_S2]->update;
    input_ids[I_S3]->status = &output_value[2];
    input_ids[I_S3]->update = &output_ids[O_S3]->update;
    input_ids[I_S4]->status = &output_value[3];
    input_ids[I_S4]->update = &output_ids[O_S4]->update;
    input_ids[I_S5]->status = &output_value[4];
    input_ids[I_S5]->update = &output_ids[O_S5]->update;
    input_ids[I_S6]->status = &output_value[5];
    input_ids[I_S6]->update = &output_ids[O_S6]->update;
    input_ids[I_S7]->status = &output_value[6];
    input_ids[I_S7]->update = &output_ids[O_S7]->update;
    input_ids[I_S8]->status = &output_value[7];
    input_ids[I_S8]->update = &output_ids[O_S8]->update;
}

void cpart_switches::Reset(void) {
    // force pin update
    if (output_pins[0] > 0)
        SpareParts.SetPin(output_pins[0], !output_value[0]);
    if (output_pins[1] > 0)
        SpareParts.SetPin(output_pins[1], !output_value[1]);
    if (output_pins[2] > 0)
        SpareParts.SetPin(output_pins[2], !output_value[2]);
    if (output_pins[3] > 0)
        SpareParts.SetPin(output_pins[3], !output_value[3]);
    if (output_pins[4] > 0)
        SpareParts.SetPin(output_pins[4], !output_value[4]);
    if (output_pins[5] > 0)
        SpareParts.SetPin(output_pins[5], !output_value[5]);
    if (output_pins[6] > 0)
        SpareParts.SetPin(output_pins[6], !output_value[6]);
    if (output_pins[7] > 0)
        SpareParts.SetPin(output_pins[7], !output_value[7]);

    if (output_pins[0] > 0)
        SpareParts.SetPin(output_pins[0], output_value[0]);
    if (output_pins[1] > 0)
        SpareParts.SetPin(output_pins[1], output_value[1]);
    if (output_pins[2] > 0)
        SpareParts.SetPin(output_pins[2], output_value[2]);
    if (output_pins[3] > 0)
        SpareParts.SetPin(output_pins[3], output_value[3]);
    if (output_pins[4] > 0)
        SpareParts.SetPin(output_pins[4], output_value[4]);
    if (output_pins[5] > 0)
        SpareParts.SetPin(output_pins[5], output_value[5]);
    if (output_pins[6] > 0)
        SpareParts.SetPin(output_pins[6], output_value[6]);
    if (output_pins[7] > 0)
        SpareParts.SetPin(output_pins[7], output_value[7]);
}

cpart_switches::~cpart_switches(void) {
    delete Bitmap;
    canvas.Destroy();
    SWBounce_end(&bounce);
}

void cpart_switches::DrawOutput(const unsigned int i) {
    switch (output[i].id) {
        case O_P1:
        case O_P2:
        case O_P3:
        case O_P4:
        case O_P5:
        case O_P6:
        case O_P7:
        case O_P8:
            canvas.SetColor(49, 61, 99);
            canvas.Rectangle(1, output[i].x1, output[i].y1, output[i].x2 - output[i].x1, output[i].y2 - output[i].y1);
            canvas.SetFgColor(255, 255, 255);
            if (output_pins[output[i].id - O_P1] == 0)
                canvas.RotatedText("NC", output[i].x1, output[i].y1, 0);
            else
                canvas.RotatedText(SpareParts.GetPinName(output_pins[output[i].id - O_P1]), output[i].x1, output[i].y1,
                                   0);
            break;
        case O_S1:
        case O_S2:
        case O_S3:
        case O_S4:
        case O_S5:
        case O_S6:
        case O_S7:
        case O_S8:
            canvas.SetFgColor(0, 0, 0);
            if (active) {
                canvas.SetBgColor(100, 60, 60);
            } else {
                canvas.SetBgColor(60, 100, 60);
            }
            canvas.Rectangle(1, output[i].x1, output[i].y1, output[i].x2 - output[i].x1, output[i].y2 - output[i].y1);

            if (active) {
                canvas.SetBgColor(60, 100, 60);
            } else {
                canvas.SetBgColor(100, 60, 60);
            }
            int w2 = (output[i].y2 - output[i].y1) / 4;

            canvas.Rectangle(1, output[i].x1, output[i].y1 + w2 + w2 * (2 * (!output_value[output[i].id - O_S1])),
                             output[i].x2 - output[i].x1, w2 + w2 * (2 * (output_value[output[i].id - O_S1])));
            canvas.SetFgColor(0, 0, 0);
            canvas.SetBgColor(26, 26, 26);
            canvas.Rectangle(1, output[i].x1 + 2, output[i].y1 + 16 - 14 * output_value[output[i].id - O_S1], 14, 14);

            break;
    }
}

void cpart_switches::PreProcess(void) {
    if (mode == MODE_NORMAL) {
        const picpin* ppins = SpareParts.GetPinsValues();
        SWBounce_prepare(&bounce, PICSimLab.GetBoard()->MGetInstClockFreq());

        for (unsigned int i = 0; i < Size; i++) {
            if (output_pins[i]) {
                unsigned char out = output_value[i];
                if (!active) {
                    out = !out;
                }
                if ((ppins[output_pins[i] - 1].dir == PD_IN) && (ppins[output_pins[i] - 1].value != out)) {
                    SWBounce_bounce(&bounce, i);
                }
            }
        }
        SetAwaysUpdate(bounce.do_bounce);
    } else {
        for (unsigned int i = 0; i < Size; i++) {
            if (output_pins[i]) {
                if (active) {
                    SpareParts.SetPin(output_pins[i], output_value[i]);
                } else {
                    SpareParts.SetPin(output_pins[i], !output_value[i]);
                }
            }
        }
        SetAwaysUpdate(0);
    }
}

void cpart_switches::Process(void) {
    if (mode == MODE_NORMAL) {
        const int ret = SWBounce_process(&bounce);
        if (ret) {
            const picpin* ppins = SpareParts.GetPinsValues();

            for (unsigned int i = 0; i < Size; i++) {
                if (bounce.bounce[i]) {
                    if (ret == 1) {
                        SpareParts.SetPin(output_pins[i], !ppins[output_pins[i] - 1].value);
                    } else {
                        if (active) {
                            SpareParts.SetPin(output_pins[i], output_value[i]);
                        } else {
                            SpareParts.SetPin(output_pins[i], !output_value[i]);
                        }
                    }
                }
            }
        }
    }
}

void cpart_switches::OnMouseButtonPress(uint inputId, uint button, uint x, uint y, uint state) {
    switch (inputId) {
        case I_S1:
            output_value[0] ^= 1;
            output_ids[O_S1]->update = 1;
            break;
        case I_S2:
            output_value[1] ^= 1;
            output_ids[O_S2]->update = 1;
            break;
        case I_S3:
            output_value[2] ^= 1;
            output_ids[O_S3]->update = 1;
            break;
        case I_S4:
            output_value[3] ^= 1;
            output_ids[O_S4]->update = 1;
            break;
        case I_S5:
            output_value[4] ^= 1;
            output_ids[O_S5]->update = 1;
            break;
        case I_S6:
            output_value[5] ^= 1;
            output_ids[O_S6]->update = 1;
            break;
        case I_S7:
            output_value[6] ^= 1;
            output_ids[O_S7]->update = 1;
            break;
        case I_S8:
            output_value[7] ^= 1;
            output_ids[O_S8]->update = 1;
            break;
    }
}

unsigned short cpart_switches::GetInputId(char* name) {
    if (strcmp(name, "SW_1") == 0)
        return I_S1;
    if (strcmp(name, "SW_2") == 0)
        return I_S2;
    if (strcmp(name, "SW_3") == 0)
        return I_S3;
    if (strcmp(name, "SW_4") == 0)
        return I_S4;
    if (strcmp(name, "SW_5") == 0)
        return I_S5;
    if (strcmp(name, "SW_6") == 0)
        return I_S6;
    if (strcmp(name, "SW_7") == 0)
        return I_S7;
    if (strcmp(name, "SW_8") == 0)
        return I_S8;

    printf("Erro input '%s' don't have a valid id! \n", name);
    return -1;
}

unsigned short cpart_switches::GetOutputId(char* name) {
    if (strcmp(name, "PN_1") == 0)
        return O_P1;
    if (strcmp(name, "PN_2") == 0)
        return O_P2;
    if (strcmp(name, "PN_3") == 0)
        return O_P3;
    if (strcmp(name, "PN_4") == 0)
        return O_P4;
    if (strcmp(name, "PN_5") == 0)
        return O_P5;
    if (strcmp(name, "PN_6") == 0)
        return O_P6;
    if (strcmp(name, "PN_7") == 0)
        return O_P7;
    if (strcmp(name, "PN_8") == 0)
        return O_P8;

    if (strcmp(name, "SW_1") == 0)
        return O_S1;
    if (strcmp(name, "SW_2") == 0)
        return O_S2;
    if (strcmp(name, "SW_3") == 0)
        return O_S3;
    if (strcmp(name, "SW_4") == 0)
        return O_S4;
    if (strcmp(name, "SW_5") == 0)
        return O_S5;
    if (strcmp(name, "SW_6") == 0)
        return O_S6;
    if (strcmp(name, "SW_7") == 0)
        return O_S7;
    if (strcmp(name, "SW_8") == 0)
        return O_S8;

    printf("Erro output '%s' don't have a valid id! \n", name);
    return 1;
}

lxString cpart_switches::WritePreferences(void) {
    char prefs[256];

    sprintf(prefs, "%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%u",
            output_pins[0], output_pins[1], output_pins[2], output_pins[3], output_pins[4], output_pins[5],
            output_pins[6], output_pins[7], output_value[0], output_value[1], output_value[2], output_value[3],
            output_value[4], output_value[5], output_value[6], output_value[7], active, mode, Size);

    return prefs;
}

void cpart_switches::ReadPreferences(lxString value) {
    unsigned int sz;
    sscanf(value.c_str(),
           "%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%u",
           &output_pins[0], &output_pins[1], &output_pins[2], &output_pins[3], &output_pins[4], &output_pins[5],
           &output_pins[6], &output_pins[7], &output_value[0], &output_value[1], &output_value[2], &output_value[3],
           &output_value[4], &output_value[5], &output_value[6], &output_value[7], &active, &mode, &sz);
    ChangeSize(sz);
}

void cpart_switches::ConfigurePropertiesWindow(CPWindow* WProp) {
    SetPCWComboWithPinNames(WProp, "combo2", output_pins[0]);
    SetPCWComboWithPinNames(WProp, "combo3", output_pins[1]);
    SetPCWComboWithPinNames(WProp, "combo4", output_pins[2]);
    SetPCWComboWithPinNames(WProp, "combo5", output_pins[3]);
    SetPCWComboWithPinNames(WProp, "combo6", output_pins[4]);
    SetPCWComboWithPinNames(WProp, "combo7", output_pins[5]);
    SetPCWComboWithPinNames(WProp, "combo8", output_pins[6]);
    SetPCWComboWithPinNames(WProp, "combo9", output_pins[7]);

    CCombo* combo = (CCombo*)WProp->GetChildByName("combo11");
    combo->SetItems("Up,Down,");
    if (active)
        combo->SetText("Up");
    else
        combo->SetText("Down");

    combo = (CCombo*)WProp->GetChildByName("combo12");
    combo->SetItems("Ideal,Normal,");
    if (mode)
        combo->SetText("Ideal");
    else
        combo->SetText("Normal");

    ((CSpin*)WProp->GetChildByName("spin13"))->SetMax(8);
    ((CSpin*)WProp->GetChildByName("spin13"))->SetMin(1);
    ((CSpin*)WProp->GetChildByName("spin13"))->SetValue(Size);
    ((CSpin*)WProp->GetChildByName("spin13"))->EvOnChangeSpin = SpareParts.PropSpinChange;
    SpinChange(WProp, NULL, Size);
}

void cpart_switches::ReadPropertiesWindow(CPWindow* WProp) {
    output_pins[0] = GetPWCComboSelectedPin(WProp, "combo2");
    output_pins[1] = GetPWCComboSelectedPin(WProp, "combo3");
    output_pins[2] = GetPWCComboSelectedPin(WProp, "combo4");
    output_pins[3] = GetPWCComboSelectedPin(WProp, "combo5");
    output_pins[4] = GetPWCComboSelectedPin(WProp, "combo6");
    output_pins[5] = GetPWCComboSelectedPin(WProp, "combo7");
    output_pins[6] = GetPWCComboSelectedPin(WProp, "combo8");
    output_pins[7] = GetPWCComboSelectedPin(WProp, "combo9");

    active = (((CCombo*)WProp->GetChildByName("combo11"))->GetText().compare("Up") == 0);

    mode = (((CCombo*)WProp->GetChildByName("combo12"))->GetText().compare("Ideal") == 0);

    ChangeSize(((CSpin*)WProp->GetChildByName("spin13"))->GetValue());
}

void cpart_switches::SpinChange(CPWindow* WProp, CSpin* control, int value) {
    for (int i = 0; i < 8; i++) {
        char name[20];
        sprintf(name, "combo%i", i + 2);
        ((CCombo*)WProp->GetChildByName(name))->SetEnable(i < value);
    }
}

void cpart_switches::ChangeSize(const unsigned int sz) {
    if (Size != sz) {
        if (Bitmap) {
            delete Bitmap;
        }
        Size = sz;
        if (Size > 8) {
            Size = 8;
        }
        outputc = Size * 2;
        inputc = Size;
        LoadImage();
    }
}

void cpart_switches::LoadImage(void) {
    if (Size < 8) {
        xoff = (8 - Size) * 49;

        Width = OWidth - xoff;
        Height = OHeight;

        if (SpareParts.GetWindow()) {
            lxImage image(SpareParts.GetWindow());
            image.CreateBlank(Width, Height, Orientation, Scale, Scale);

            Bitmap = new lxBitmap(&image, SpareParts.GetWindow());
            image.Destroy();

            canvas.Destroy();
            canvas.Create(SpareParts.GetWindow()->GetWWidget(), Bitmap);

            image.LoadFile(lxGetLocalFile(PICSimLab.GetSharePath() + lxT("parts/") + Type + "/" + GetPictureFileName()),
                           Orientation, Scale, Scale);
            lxBitmap* BackBitmap = new lxBitmap(&image, SpareParts.GetWindow());
            image.Destroy();

            canvas.Init(Scale, Scale, Orientation);
            canvas.SetColor(0x31, 0x3d, 0x63);
            canvas.Rectangle(1, 0, 0, Width, Height);

            canvas.ChangeScale(1.0, 1.0);
            canvas.PutBitmap(BackBitmap, -xoff * Scale, 0);

            canvas.ChangeScale(Scale, Scale);
            canvas.End();

            delete BackBitmap;
        }
    } else {
        Width = OWidth;
        Height = OHeight;
        xoff = 0;
        part::LoadImage();
    }
}

part_init(PART_SWITCHES_Name, cpart_switches, "Input");
