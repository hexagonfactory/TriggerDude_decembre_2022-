void drawPage_startupScreen() {
  u8g2.setFont(u8g2_font_pressstart2p_8u);
  u8g2.setCursor(14, 22);
  u8g2.print("TRIGGER DUDE");

  u8g2.setFont(triggerDude_small_font);
  u8g2.setCursor(14, 60);
  u8g2.print(FW_VERSION);

  u8g2.drawFrame(20, 30, 88, 10);

  for (byte x = 0; x < 85; ++x) {
    u8g2.drawBox(22, 32, x, 6 );
    u8g2.sendBuffer();
    delay(10);
  }
}



void set_default_UI_header(const char* screenModeName) {
  u8g2.clearBuffer();
  u8g2.setFont(triggerDude_big_font);
  u8g2.setCursor(0, 7);
  u8g2.print(screenModeName);

  u8g2.drawHLine(0, 9, 128);

  update_screen_TOP = true;
  update_screen_POTS_DIAL_L = true;
  update_screen_POTS_DIAL_R = true;
  update_screen_POTS_CLK_L = true;
  update_screen_POTS_CLK_R = true;
  update_screen_PADS = true;
  update_screen_ALT_BTN = true;
}


void set_mainScreen_UI_header() {
  u8g2.clearBuffer();
  u8g2.setFont(triggerDude_big_font);

  update_screen_TOP = true;
  update_screen_POTS_DIAL_L = true;
  update_screen_POTS_DIAL_R = true;
  update_screen_POTS_CLK_L = true;
  update_screen_POTS_CLK_R = true;
  update_screen_PADS = true;
  update_screen_ALT_BTN = true;
}


void setFontMode_BG(bool x) {
  u8g2.setFontMode(x);
  u8g2.setDrawColor(x);
}


void update_TOP() {
  if (update_screen_TOP) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    display_global_Step_4PPQN();
    update_screen_TOP = false;

    if (!update_screen_INIT)
      u8g2.updateDisplayArea(10, 0, 6, 1);
  }
}


void update_on_INIT() {
  if (update_screen_INIT) {
    u8g2.sendBuffer();
    update_screen_INIT = false;
  }
}


void drawPage_mainScreen() {
  if (update_screen_INIT) {
    set_mainScreen_UI_header();

    if (SYNC_mode_internal)
      display_BPM(0, 12);

    //drawBloc_CvTracks(0, 12);

    /*u8g2.print("MODE: ");
      (songMode_Activated) ? u8g2.print("SONG") : u8g2.print("PATTERN");*/

    u8g2.drawRFrame(99, 17, 25, 25, 2); // TRACK FRAME
    u8g2.setCursor(104, 27);
    u8g2.print("TRK");

    display_enc1_dial("RATE /", 1);


  }


  if (update_screen_TOP) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    drawSequence_avancement();
    display_global_Step_4PPQN();
    u8g2.drawHLine(0, 9, 20);

    display_BPM(0, 12);

    update_screen_TOP = false;
    if (!update_screen_INIT) {
      u8g2.updateDisplayArea(0, 0, 16, 1);
      if (!SYNC_mode_internal)
        u8g2.updateDisplayArea(0, 1, 6, 2);
    }
  }


  if (update_screen_PADS) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    byte selTrack = selected_Track + 1;
    (selTrack < 10) ? u8g2.setCursor(109, 37) : u8g2.setCursor(106, 37);
    u8g2.setFont(triggerDude_small_font);
    u8g2.print(selTrack);

    if (ALT_pushed) {
      update_screen_ALT_BTN = true; // on relance if (update_screen_ALT_BTN)

    }

    if (!update_screen_INIT)
      u8g2.updateDisplayArea(13, 4, 2, 1);

    update_screen_PADS = false;
  }


  if (update_screen_ALT_BTN) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    if (!ALT_pushed) {
      if (!(CVin[0].enable && CVin[0].dest_Selected == 2) && !(CVin[1].enable && CVin[1].dest_Selected == 2)) {
        display_enc2_dial(94, "STEPS", 1);
        (pattern_Length_Locked) ? display_enc2_click(89, "UNLOCK") : display_enc2_click(99, "LOCK");
      }
    }

    else {
      display_enc2_dial(99, "TYPE", 1);
      u8g2.setCursor(99, 63);
      (bank[currentBank].pattern[currentPattern].track_isGate[selected_Track]) ? u8g2.print("GATE") : u8g2.print("TRIG");
    }

    update_screen_ALT_BTN = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(11, 6, 5, 2);
  }


  if (update_screen_POTS_DIAL_L) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    u8g2.setCursor(45, 55);
    u8g2.setFont(triggerDude_small_font);
    u8g2.print(rollRates[rollRates_cursor]);

    update_screen_POTS_DIAL_L = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(5, 6, 2, 1);
  }


  if (update_screen_POTS_CLK_L) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    (rollMode) ? display_enc1_click("ROLL OFF") : display_enc1_click("ROLL ON");

    update_screen_POTS_CLK_L = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(0, 7, 5, 1);
  }


  if (update_screen_POTS_DIAL_R && screenConfig == 1 /* !rollMode ?*/) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    if (!ALT_pushed) {
      if (!update_screen_INIT)
        update_screen_TOP = true;
    }

    else {
      u8g2.setCursor(99, 63);
      u8g2.setFont(triggerDude_small_font);
      (bank[currentBank].pattern[currentPattern].track_isGate[selected_Track]) ? u8g2.print("GATE") : u8g2.print("TRIG");
      if (!update_screen_INIT)
        u8g2.updateDisplayArea(12, 7, 4, 1);
    }

    update_screen_POTS_DIAL_R = false;

  }


  if (update_screen_POTS_CLK_R && screenConfig == 1 && !ALT_pushed && !(CVin[0].enable && CVin[0].dest_Selected == 2) && !(CVin[1].enable && CVin[1].dest_Selected == 2)) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();
    (pattern_Length_Locked) ? display_enc2_click(89, "UNLOCK") : display_enc2_click(99, "LOCK");

    update_screen_POTS_CLK_R = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(11, 7, 5, 1);
  }


  update_on_INIT();
}


void drawPage_Settings() {
  if (update_screen_INIT) {
    set_default_UI_header("SETTINGS");

    display_enc1_dial("SELECT", 1);
    display_enc1_click("EXIT");
    display_enc2_dial(95, "VALUE", 1);
  }


  if (update_screen_POTS_DIAL_L || update_screen_POTS_DIAL_R || update_screen_POTS_CLK_R) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    u8g2.drawHLine(0, 9, 128);
    draw_Settings_list();
    u8g2.drawHLine(0, 46, 128);

    if (!update_screen_INIT) {
      //if (!update_screen_POTS_CLK_R)
      u8g2.updateDisplayArea(0, 1, 16, 5);
      u8g2.updateDisplayArea(8, 7, 8, 1);
    }

    update_screen_POTS_DIAL_L = false;
    update_screen_POTS_DIAL_R = false;
    update_screen_POTS_CLK_R = false;
    //update_screen_ONBEAT_4x4 = false;
  }


  update_on_INIT();
}


void drawSequence_avancement() {
  /* u8g2.drawFrame(0, 0, 65, 7); // AVANCEMENT FRAME

    for (byte x = 16; x <= 48; x += 16) {
     u8g2.drawVLine(x, 2, 3);
    }

    uint16_t global_Step_4PPQN_tracker = (displayFirstStep_1) ? 0 : get_current_Step_Display_4PPQN(1) * (64 / bank[currentBank].pattern[currentPattern].pattern_Length);

    if (bank[currentBank].pattern[currentPattern].pattern_Length == 128)
     global_Step_4PPQN_tracker = (displayFirstStep_1) ? 0 : get_current_Step_Display_4PPQN(1) / 2;

    u8g2.setDrawColor(2);
    u8g2.drawBox(1, 1, global_Step_4PPQN_tracker, 5);
    u8g2.setDrawColor(1);*/

  u8g2.setCursor(4, 7);
  u8g2.print(rollRate_micros);
  u8g2.print(" / ");
  u8g2.print(delayBeforeTrig);
}


void display_BPM(byte pix_x, byte pix_y) {
  draw_3D_box(pix_x, pix_y, 20, 14, "BPM", 23);
  makeSpaceBeforeBpm(bpm);
  u8g2.print(bpm);
}


void display_track_or_pitch() {
  byte dataToDisplay = selected_Track + 1;
  if (dataToDisplay < 10) u8g2.print(" ");

  u8g2.print(dataToDisplay);
  if (selected_Track + step_seq_ab[selected_Track] > 11)
    u8g2.print("B");
}


void drawPage_stepSequencer() { // ---- STEP SEQUENCER
  if (update_screen_INIT) {
    set_default_UI_header("STEP SEQ");

    drawBloc_CvTracks(59, 12);

    display_enc1_dial("TRACK", 1);
    display_enc1_click("A / B");

    display_enc2_dial(99, "PAGE", 1);
    display_enc2_click(89, "PARAMS");
  }


  update_TOP();


  if (update_screen_POTS_DIAL_L || update_screen_POTS_CLK_R) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    draw_pads_UI(1);

    drawBloc_selected_Track(37, 16, 0);

    update_screen_POTS_DIAL_L = false;
    if (!update_screen_INIT) {
      u8g2.updateDisplayArea(5, 3, 2, 2); // TRACK selected BOX
      u8g2.updateDisplayArea(0, 2, 5, 4); // PADS UI
    }

  }


  if (update_screen_POTS_DIAL_R) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    drawBloc_SeqPage(59, 30);

    update_screen_POTS_DIAL_R = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(11, 4, 4, 1);
  }


  /*if (update_screen_POTS_CLK_R) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    update_screen_POTS_CLK_R = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(11, 7, 5, 1);
    }*/


  update_on_INIT();
}


void drawPage_stepSequencer_params() {
  if (update_screen_INIT) {
    set_default_UI_header("STEP PARAMS");

    display_enc1_dial("SELECT  ", 1);

    display_enc2_dial(89, "VALUE", 1);
    display_enc2_click(89, "EXIT");
  }

  update_TOP();

  //drawBloc_selected_Track(37, 14, 0);
  //drawBloc_CvTracks(59, 12);
  //drawBloc_SeqPage(59, 31);

  update_on_INIT();
}


void drawPage_pitchSequencer() { // ---- PITCH SEQUENCER
  if (update_screen_INIT) {
    set_default_UI_header("PITCH SEQ");

    drawBloc_selected_Track(37, 16, 0);
    drawBloc_CvTracks(59, 12);

    display_enc2_dial(99, "PAGE", 1);
    //display_enc2_click(74, "STEP MODE");
  }


  update_TOP();


  if (update_screen_PADS || update_screen_POTS_DIAL_L || update_screen_POTS_DIAL_R) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    if (stepSeq_clickedStep != 255) {
      draw_pitch_UI();

      if (update_screen_PADS || update_screen_POTS_DIAL_R) {
        display_enc1_dial("PITCH", 1);
        (pitch_Range_12) ? display_enc1_click("SEMITONE") : display_enc1_click("OCTAVE");
      }
    }

    if (update_screen_POTS_DIAL_R)
      drawBloc_SeqPage(59, 30);

    if (!update_screen_INIT) {
      u8g2.updateDisplayArea(0, 2, 4, 4); // PITCH UI

      if (update_screen_PADS || update_screen_POTS_DIAL_R) {
        u8g2.updateDisplayArea(0, 6, 5, 2); // POT L - dial + clk

        if (update_screen_POTS_DIAL_R)
          u8g2.updateDisplayArea(11, 4, 4, 1); // page num
      }
    }

    update_screen_PADS = false;
    update_screen_POTS_DIAL_L = false;
    update_screen_POTS_DIAL_R = false;
  }


  if (update_screen_POTS_CLK_L) {
    if (!update_screen_INIT && stepSeq_clickedStep != 255) {
      u8g2.clearBuffer();

      (pitch_Range_12) ? display_enc1_click("SEMITONE") : display_enc1_click("OCTAVE");

      u8g2.updateDisplayArea(0, 7, 6, 1);

      update_screen_POTS_CLK_L = false;
    }
  }


  update_on_INIT();
}


void drawPage_glideMode() {
  if (update_screen_INIT) {
    set_default_UI_header("GLIDE");

    display_enc1_dial("LENGTH", 1);
    display_enc1_click("EXIT");
  }


  if (update_screen_POTS_DIAL_L) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    draw_glide_UI();

    update_screen_POTS_DIAL_L = false;
    if (!update_screen_INIT) {
      u8g2.updateDisplayArea(5, 6, 2, 1);
    }
  }


  /*//DEBUG
    u8g2.setFont(triggerDude_small_font);
    u8g2.setCursor(48, 20);
    u8g2.print("START   ");
    u8g2.print(DaC[0].startPitch);
    u8g2.setCursor(48, 28);
    u8g2.print("END     ");
    u8g2.print(DaC[0].endPitch);
    u8g2.setCursor(48, 36);
    u8g2.print("PULSE   ");
    u8g2.print(glidePulse);
    u8g2.setCursor(48, 44);
    u8g2.print("INCREM  ");
    u8g2.print(DaC[0].incrementValue);
    u8g2.setCursor(48, 52);
    u8g2.print("FLAG   ");
    u8g2.setCursor(48, 60);
    u8g2.print("VAL ");
    u8g2.print(DaC[0].currentPitch);*/


  update_on_INIT();
}


void drawPage_bankSelection() {
  if (update_screen_INIT) {
    set_default_UI_header("BANK");

    display_CurrentPattern(0, 27);

    display_enc1_click("EDIT");
  }


  update_TOP();


  if (update_screen_PADS) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    display_CurrentPattern(0, 27);

    update_screen_PADS = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(6, 2, 2, 1);
  }


  update_on_INIT();
}


void drawPage_Pattern_Selection() {
  if (update_screen_INIT) {
    set_default_UI_header("PATTERN");

    display_CurrentBank(0, 12);

    display_enc1_click("EDIT");
    display_enc2_click(74, "SONG MODE");
  }


  update_TOP();


  if (update_screen_PADS) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    display_CurrentPattern(0, 28);

    update_screen_PADS = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(6, 4, 2, 1);
  }


  update_on_INIT();
}


void drawPage_patternEditMode() {
  if (update_screen_INIT) {
    set_default_UI_header("EDIT PATTERN");
  }


  update_TOP();


  if (update_screen_PADS || update_screen_POTS_DIAL_R || update_screen_POTS_CLK_R || update_screen_POTS_CLK_L) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    display_patternEdit_things();

    update_screen_PADS = false;
    update_screen_POTS_DIAL_R = false;
    update_screen_POTS_CLK_R = false;
    update_screen_POTS_CLK_L = false;

    if (!update_screen_INIT) {
      //u8g2.updateDisplayArea(0, 2, 4, 3);
      //u8g2.updateDisplayArea(12, 7, 4, 1);

      u8g2.updateDisplayArea(0, 2, 16, 6);
    }
  }


  update_on_INIT();
}


void drawPage_bankEditMode() {
  set_default_UI_header("EDIT BANK");

  display_bankEdit_things();
  display_enc1_click("EXIT");

  u8g2.sendBuffer();
}


void drawPage_songMode() {
  if (update_screen_INIT) {
    set_default_UI_header("EDIT SONG");

    display_enc1_dial("STEP", 1);
    display_enc2_click(99, "EXIT");
  }


  update_TOP();


  if (update_screen_PADS || update_screen_POTS_DIAL_L) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    display_songMode_Grid();

    update_screen_PADS = false;
    update_screen_POTS_DIAL_L = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(13, 4, 2, 1);
  }

  if (update_screen_POTS_CLK_L) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    (songMode_Activated) ? display_enc1_click("DEACTIVATE") : display_enc1_click("ACTIVATE");

    update_screen_POTS_CLK_L = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(11, 7, 5, 1);
  }


  update_on_INIT();
}


void drawPage_ABstate() {
  if (update_screen_INIT) {
    set_default_UI_header("A/B");

    display_enc1_dial("A/B", 1);
    display_enc1_click("All ");
    //display_enc1_click("RESET");

    display_enc2_click(79, "SWAP A/B");
  }


  update_TOP();


  if (update_screen_PADS || update_screen_POTS_CLK_L || update_screen_POTS_CLK_R) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    draw_pads_UI(0);

    if (!update_screen_INIT)
      u8g2.updateDisplayArea(0, 1, 5, 4);

    update_screen_PADS = false;
    update_screen_POTS_CLK_L = false;
    update_screen_POTS_CLK_R = false;
  }


  if (update_screen_POTS_DIAL_L) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    u8g2.setCursor(30, 63);
    (bank[currentBank].pattern[currentPattern].allAorB) ? u8g2.print("B") : u8g2.print("A");

    update_screen_POTS_DIAL_L = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(3, 7, 2, 1);
  }


  update_on_INIT();
}


void drawPage_loopLength() {
  if (update_screen_INIT) {
    set_default_UI_header("LOOP LENGTH");

    u8g2.setFont(triggerDude_big_font);

    u8g2.setCursor(6, 23);
    u8g2.print("TRACK");

    u8g2.drawVLine(64, 18, 30);

    u8g2.setCursor(75, 23);
    u8g2.print("PATTERN");

    u8g2.setCursor(80, 40);
    u8g2.setFont(u8g2_font_pressstart2p_8u);
    if (!bank[currentBank].pattern[currentPattern].tracksRetrig)
      u8g2.print("FREE");

    display_enc1_dial("LENGTH", 1);

    if (bank[currentBank].pattern[currentPattern].tracksRetrig) {
      if (!(CVin[0].enable && CVin[0].dest_Selected == 2) && !(CVin[1].enable && CVin[1].dest_Selected == 2)) {
        display_enc2_dial(89, "LENGTH", 1);
      }
    }
  }


  update_TOP();


  if (update_screen_PADS) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    u8g2.setCursor(40, 23);
    u8g2.setFont(triggerDude_big_font);
    u8g2.print(selected_Track + 1);

    update_screen_POTS_DIAL_L = true;

    if (!update_screen_INIT)
      u8g2.updateDisplayArea(5, 2, 2, 1);

    update_screen_PADS = false;
  }


  if (update_screen_POTS_DIAL_L) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    u8g2.setFont(u8g2_font_pressstart2p_8u);
    byte trackLgth = bank[currentBank].pattern[currentPattern].track_Length[selected_Track];
    if (trackLgth < 10)
      u8g2.setCursor(24, 40);
    else if (trackLgth < 100)
      u8g2.setCursor(20, 40);
    else
      u8g2.setCursor(16, 40);
    u8g2.print(trackLgth);


    update_screen_POTS_DIAL_L = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(1, 4, 4, 1);
  }


  if (update_screen_POTS_DIAL_R) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    if (bank[currentBank].pattern[currentPattern].tracksRetrig) {
      byte patLgth = bank[currentBank].pattern[currentPattern].pattern_Length;
      if (patLgth < 10)
        u8g2.setCursor(94, 40);
      else if (patLgth < 100)
        u8g2.setCursor(90, 40);
      else
        u8g2.setCursor(86, 40);
      u8g2.setFont(u8g2_font_pressstart2p_8u);
      u8g2.print(patLgth);
    }

    update_screen_POTS_DIAL_R = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(10, 4, 4, 1);

  }


  /*if (bank[currentBank].pattern[currentPattern].tracksRetrig) {
    if (!(CVin[0].enable && CVin[0].dest_Selected == 2) && !(CVin[1].enable && CVin[1].dest_Selected == 2)) {
      display_enc2_dial(80, "LENGTH", 1);
      (pattern_Length_Locked) ? display_enc2_click(89, "UNLOCK") : display_enc2_click(89, "  LOCK");
    }
    }*/


  if (update_screen_POTS_CLK_L) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    display_enc1_click((enc1_Range_10)  ? "+/- 1" : "+/- 10");

    update_screen_POTS_CLK_L = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(0, 7, 5, 1);
  }


  if (update_screen_POTS_CLK_R) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    if (bank[currentBank].pattern[currentPattern].tracksRetrig) {
      if (!(CVin[0].enable && CVin[0].dest_Selected == 2) && !(CVin[1].enable && CVin[1].dest_Selected == 2)) {
        (pattern_Length_Locked) ? display_enc2_click(89, "UNLOCK") : display_enc2_click(99, "LOCK");
      }
    }

    update_screen_POTS_CLK_R = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(11, 7, 5, 1);
  }

  update_on_INIT();
}


void drawPage_muteState() {
  if (update_screen_INIT) {
    set_default_UI_header("MUTE");
  }

  update_TOP();


  if (update_screen_PADS || update_screen_POTS_CLK_L || update_screen_POTS_CLK_R) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    if (bank[currentBank].pattern[currentPattern].mute_active)
      display_enc1_click("Reset");

    if (!bank[currentBank].pattern[currentPattern].allMute_active)
      display_enc2_click(80, "MUTE ALL");

    update_screen_PADS = false;
    update_screen_POTS_CLK_L = false;
    update_screen_POTS_CLK_R = false;

    if (!update_screen_INIT)
      u8g2.updateDisplayArea(0, 7, 16, 1);
  }

  update_on_INIT();
}


void drawPage_soloState() {
  if (update_screen_INIT) {
    set_default_UI_header("SOLO");
  }

  update_TOP();

  if (update_screen_PADS || update_screen_POTS_CLK_L) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    if (bank[currentBank].pattern[currentPattern].solo_active)
      display_enc1_click("Reset");

    update_screen_PADS = false;
    update_screen_POTS_CLK_L = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(0, 7, 4, 1);
  }

  update_on_INIT();
}


void drawPage_fillState() {
  if (update_screen_INIT) {
    set_default_UI_header("FILL");

    u8g2.drawRFrame(99, 17, 25, 25, 2); // TRACK FRAME
    u8g2.setCursor(104, 27);
    u8g2.print("TRK");

    /*u8g2.setFont(u8g2_font_pressstart2p_8u);
      u8g2.setCursor(52, 24);
      u8g2.print("RATE");*/

    display_enc1_dial("RATE", 1);
  }

  update_TOP();

  if (update_screen_PADS) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    byte selTrack = selected_Track + 1;
    (selTrack < 10) ? u8g2.setCursor(109, 37) : u8g2.setCursor(106, 37);
    u8g2.setFont(triggerDude_small_font);
    u8g2.print(selTrack);

    update_screen_POTS_DIAL_L = true; // on lance la condition

    if (!update_screen_INIT)
      u8g2.updateDisplayArea(13, 4, 2, 1);

    update_screen_PADS = false;
  }


  if (update_screen_POTS_DIAL_L) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    display_curTrack_FILLRATE();

    update_screen_POTS_DIAL_L = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(8, 3, 4, 2);
  }


  if (update_screen_POTS_CLK_L) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    update_screen_POTS_CLK_L = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(0, 7, 5, 1);
  }


  if (update_screen_POTS_CLK_R) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    if (bank[currentBank].pattern[currentPattern].fill_active)
      (recMode) ? display_enc1_click("STOP REC") : display_enc1_click("REC TO SEQ");


    update_screen_POTS_CLK_R = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(11, 7, 5, 1);
  }


  //display_enc1_dial("RATE", 1);


  update_on_INIT();
}


/*void drawPage_chromaMode() {
  if (update_screen_INIT) {
    set_default_UI_header("CHROMA MODE");
    //drawBloc_selected_Track(48, 14, 1);
  }


  if (update_screen_TOP) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    display_global_Step_4PPQN();

    update_screen_TOP = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(10, 0, 6, 1);
  }




  if (!trigger_cvOutReassign_process) {
    if (chromaMode_Arp_params == 0)
      drawBloc_selected_Track(68, 24, 1);
    displayChromaMode_title();
  }


  if (chromaMode_KBoard) {
    if (glideMode) {
      draw_glide_UI();
      display_enc1_dial("LENGTH", 1);
      display_enc1_click("EXIT");
    }

    else {
      displayOctaveValue();
      display_enc1_dial("OCTAVE", 1);
      display_enc1_click("GLIDE");
    }
  }

  else if (chromaMode_Arp) {
    if (chromaMode_Arp_params == 0) {
      displayOctaveValue();
      display_enc1_dial("OCTAVE", 1);
      display_enc1_click("PARAMS");
    }

    else {
      draw_arp_UI();
      display_enc1_dial("PARAM", 1);
      display_enc1_click("EXIT");
    }

    if (arp_latch)
      display_enc2_click(75, "LATCH OFF");
    else
      display_enc2_click(80, "LATCH ON");


    u8g2.setCursor(50, 22);
    u8g2.print(arpSteps);
    u8g2.setCursor(50, 32);
    u8g2.print(arp_latch_active);

    u8g2.setCursor(50, 42);
    u8g2.print(arp_Style_Cursor);
    u8g2.setCursor(50, 52);
    u8g2.print(scan_pads_Arp);
    u8g2.setCursor(50, 62);
    u8g2.print(needToCheck_Arp);
  }

  else if (chromaMode_Transpose) {
    displayTransposeValue();
    display_enc1_dial("OCTAVE", 1);

    if (bank[currentBank].pattern[currentPattern].transposeValue[selected_Track + bank[currentBank].pattern[currentPattern].AB_State[selected_Track]] != 0)
      display_enc1_click("RESET");
  }

  else if (chromaMode_Glide) {
    byte currentBankDisplay = get_currentBank_Display(0);
    byte currentPatternDisplay = get_currentPattern_Display(0);

    u8g2.setCursor(3, 21);
    u8g2.print("MODE:");
    if (GLIDE_paramSelection_cursor == 0) {
      u8g2.drawFrame(0, 12, 64, 13);
      if (GLIDE_paramSelection_active) {
        u8g2.drawBox(32, 14, 30, 9);
        setFontMode_BG(0);
      }
    }

    u8g2.setCursor(34, 21);
    u8g2.print(chromaMode_Glide_params_labels[bank[currentBankDisplay].pattern[currentPatternDisplay].glide_mode[selected_Track]]);
    if (GLIDE_paramSelection_cursor == 0)
      setFontMode_BG(1);

    u8g2.setCursor(3, 34);
    u8g2.print("TIME:");
    if (GLIDE_paramSelection_cursor == 1) {
      u8g2.drawFrame(0, 25, 64, 13);
      if (GLIDE_paramSelection_active) {
        u8g2.drawBox(32, 27, 30, 9);
        setFontMode_BG(0);
      }
    }
    u8g2.setCursor(44, 34);
    u8g2.print(bank[currentBankDisplay].pattern[currentPatternDisplay].glide_steps[selected_Track]);
    if (GLIDE_paramSelection_cursor == 1)
      setFontMode_BG(1);


    if (GLIDE_paramSelection_active) {
      display_enc1_dial("VALUE", 1);
      display_enc1_click("OK");
    }
    else {
      display_enc1_dial("PARAM", 1);
      display_enc1_click("SELECT");
    }
  }

  else if (chromaMode_CVOutSelect) {
    if (!trigger_cvOutReassign_process) {
      drawBloc_chromaSettings_CVOutSelect();
      display_enc1_dial("OUTPUT", 1);
      confirmed_CvOutReassign = 0;
      //display_enc1_click("ASSIGN");
    }

    else {
      u8g2.setCursor(15, 34);
      u8g2.print("REASSIGN TO TRACK ");
      u8g2.print(selected_Track);
      u8g2.print(" ?");
      display_enc1_click("NO");
      display_enc2_click(105, "YES");
    }
  }

  if (!trigger_cvOutReassign_process) {
    if (chromaMode_Arp_params == 0)
      display_enc2_dial(100, "MODE", 1);
    else
      display_enc2_dial(95, "VALUE", 1);
  }


  if (update_screen_INIT) {
    u8g2.sendBuffer();
    update_screen_INIT = false;
  }
  }*/

void drawPage_chromaMode_KBoard() {
  if (update_screen_INIT) {
    set_default_UI_header("CHROMA MODE");
    displayChromaMode_title();

    //drawBloc_selected_Track(48, 14, 1);
    display_enc1_dial("OCTAVE", 1);
    display_enc2_dial(100, "MODE", 1);
  }


  update_TOP();


  if (update_screen_POTS_DIAL_L) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    displayOctaveValue();

    update_screen_POTS_DIAL_L = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(0, 2, 5, 3);
  }


  update_on_INIT();
}


void drawPage_chromaMode_Arp() {
  if (update_screen_INIT) {
    set_default_UI_header("CHROMA MODE");
    displayChromaMode_title();
    display_enc2_dial(100, "MODE", 1);
    //drawBloc_selected_Track(48, 14, 1);
  }


  update_TOP();


  if (update_screen_POTS_CLK_L) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    if (!chromaMode_Arp_params) {
      display_enc1_dial("OCTAVE", 1);
      display_enc1_click("PARAMS");
    }

    else {
      display_enc1_dial("PARAM", 1);
      display_enc1_click("EXIT");

      display_enc2_dial(95, "VALUE", 1);
    }

    update_screen_POTS_DIAL_L = true;

    update_screen_POTS_CLK_L = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(0, 6, 6, 2);
    u8g2.updateDisplayArea(0, 7, 16, 2);
  }


  if (update_screen_POTS_DIAL_L) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    if (!chromaMode_Arp_params)
      displayOctaveValue();

    else
      draw_arp_UI();

    update_screen_POTS_DIAL_L = false;
    if (!update_screen_INIT) {
      if (!chromaMode_Arp_params)
        u8g2.updateDisplayArea(0, 2, 5, 3);
      else
        u8g2.updateDisplayArea(0, 2, 8, 3);
    }
  }


  if (update_screen_POTS_DIAL_R) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    if (chromaMode_Arp_params)
      draw_arp_UI();

    update_screen_POTS_DIAL_R = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(5, 2, 3, 3);
  }


  if (update_screen_POTS_CLK_R) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    if (arp_latch)
      display_enc2_click(75, "LATCH OFF");
    else
      display_enc2_click(80, "LATCH ON");

    update_screen_POTS_CLK_R = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(10, 7, 6, 1);
  }


  update_on_INIT();
}


void drawPage_chromaMode_Transpose() {
  if (update_screen_INIT) {
    set_default_UI_header("CHROMA MODE");
    displayChromaMode_title();

    display_enc1_dial("OCTAVE", 1);
    display_enc2_dial(100, "MODE", 1);
    //drawBloc_selected_Track(48, 14, 1);
  }


  update_TOP();


  if (update_screen_PADS || update_screen_POTS_DIAL_L || update_screen_POTS_CLK_L) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    displayTransposeValue();

    if (bank[currentBank].pattern[currentPattern].transposeValue[selected_Track + bank[currentBank].pattern[currentPattern].AB_State[selected_Track]] != 0)
      display_enc1_click("RESET");

    update_screen_PADS = false;
    update_screen_POTS_DIAL_L = false;
    update_screen_POTS_CLK_L = false;
    if (!update_screen_INIT) {
      u8g2.updateDisplayArea(0, 2, 5, 3);
      u8g2.updateDisplayArea(0, 7, 5, 1);
    }
  }


  update_on_INIT();
}


void drawPage_chromaMode_Glide() {
  if (update_screen_INIT) {
    set_default_UI_header("CHROMA MODE");
    displayChromaMode_title();
    display_enc2_dial(100, "MODE", 1);
    //drawBloc_selected_Track(48, 14, 1);
  }


  update_TOP();


  if (update_screen_POTS_DIAL_L) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    draw_Chroma_GLIDE_UI();

    update_screen_POTS_DIAL_L = false;
    if (!update_screen_INIT) {
      u8g2.updateDisplayArea(0, 1, 8, 4);
    }
  }


  if (update_screen_POTS_CLK_L) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    draw_Chroma_GLIDE_UI();

    if (GLIDE_paramSelection_active) {
      display_enc1_dial("VALUE", 1);
      display_enc1_click("OK");
    }
    else {
      display_enc1_dial("PARAM", 1);
      display_enc1_click("SELECT");
    }

    update_screen_POTS_CLK_L = false;
    if (!update_screen_INIT) {
      u8g2.updateDisplayArea(0, 1, 8, 4);
      u8g2.updateDisplayArea(0, 7, 5, 1);
    }
  }


  update_on_INIT();
}


void drawPage_chromaMode_CVOutSelect() {
  if (update_screen_INIT) {
    set_default_UI_header("CHROMA MODE");
    display_enc2_dial(100, "MODE", 1);
    //drawBloc_selected_Track(48, 14, 1);
  }


  update_TOP();


  if (update_screen_PADS) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    if (!trigger_cvOutReassign_process) {
      drawBloc_chromaSettings_CVOutSelect();
      display_enc1_dial("OUTPUT", 1);
      confirmed_CvOutReassign = 0;
    }

    else {
      u8g2.setCursor(15, 34);
      u8g2.print("REASSIGN TO TRACK ");
      u8g2.print(selected_Track);
      u8g2.print(" ?");
      display_enc1_click("NO");
      display_enc2_click(105, "YES");
    }

    update_screen_PADS = false;
    if (!update_screen_INIT) {
      u8g2.updateDisplayArea(0, 2, 6, 3);
      if (!trigger_cvOutReassign_process)
        u8g2.updateDisplayArea(0, 7, 5, 1);
      else
        u8g2.updateDisplayArea(0, 7, 16, 1);
    }
  }


  update_on_INIT();
}


void drawPage_shiftSteps() {
  if (update_screen_INIT) {
    set_default_UI_header("SHIFT");

    display_enc1_dial("SHIFT", 1);
  }


  update_TOP();


  if (update_screen_PADS || update_screen_POTS_DIAL_L) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    draw_SHIFT_UI();

    update_screen_POTS_CLK_R = true;

    update_screen_PADS = false;
    if (!update_screen_INIT) {
      u8g2.updateDisplayArea(0, 2, 16, 3);
      u8g2.updateDisplayArea(0, 7, 4, 1);
    }
  }


  if (update_screen_POTS_CLK_R) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    if (bank[currentBank].pattern[currentPattern].shift_active)
      display_enc2_click(76, "RESET ALL");

    update_screen_POTS_CLK_R = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(9, 7, 7, 1);
  }


  update_on_INIT();
}


void drawPage_deleteSteps() {
  if (update_screen_INIT) {
    set_default_UI_header("DELETE STEPS");

    u8g2.drawVLine(63, 14, 26);

    display_enc1_dial("TRIGS/PITCH", 1);
    display_enc2_dial(105, "A/B", 1);
  }

  update_TOP();

  draw_CLEAR_DELETE_UI();

  update_on_INIT();
}


void drawPage_clearTrack() {
  if (update_screen_INIT) {
    set_default_UI_header("CLEAR TRACK");

    u8g2.drawVLine(63, 14, 26);

    display_enc1_dial("TRIGS/PITCH", 1);
    display_enc2_dial(105, "A/B", 1);
  }

  update_TOP();

  draw_CLEAR_DELETE_UI();

  update_on_INIT();
}



void draw_pads_UI(bool showOnlyselected_Track) {
  for (byte y = 16; y < 36; y += 9) {
    for (byte x = 0; x < 28; x += 9) {
      u8g2.drawFrame(x, y, 7, 7);
    }
  }


  if (showOnlyselected_Track) {
    byte AB_State = step_seq_ab[selected_Track];
    switch (selected_Track) {
      case 0:
        (AB_State == 12) ? u8g2.drawBox(2, 36, 3, 3) : u8g2.drawBox(0, 34, 7, 7);
        break;

      case 1:
        (AB_State == 12) ? u8g2.drawBox(11, 36, 3, 3) : u8g2.drawBox(9, 34, 7, 7);
        break;

      case 2:
        (AB_State == 12) ? u8g2.drawBox(20, 36, 3, 3) : u8g2.drawBox(18, 34, 7, 7);
        break;

      case 3:
        (AB_State == 12) ? u8g2.drawBox(29, 36, 3, 3) : u8g2.drawBox(27, 34, 7, 7);
        break;

      case 4:
        (AB_State == 12) ? u8g2.drawBox(2, 27, 3, 3) : u8g2.drawBox(0, 25, 7, 7);
        break;

      case 5:
        (AB_State == 12) ? u8g2.drawBox(11, 27, 3, 3) : u8g2.drawBox(9, 25, 7, 7);
        break;

      case 6:
        (AB_State == 12) ? u8g2.drawBox(20, 27, 3, 3) : u8g2.drawBox(18, 25, 7, 7);
        break;

      case 7:
        (AB_State == 12) ? u8g2.drawBox(29, 27, 3, 3) : u8g2.drawBox(27, 25, 7, 7);
        break;

      case 8:
        (AB_State == 12) ? u8g2.drawBox(2, 18, 3, 3) : u8g2.drawBox(0, 16, 7, 7);
        break;

      case 9:
        (AB_State == 12) ? u8g2.drawBox(11, 18, 3, 3) : u8g2.drawBox(9, 16, 7, 7);
        break;

      case 10:
        (AB_State == 12) ? u8g2.drawBox(20, 18, 3, 3) : u8g2.drawBox(18, 16, 7, 7);
        break;

      case 11:
        (AB_State == 12) ? u8g2.drawBox(29, 18, 3, 3) : u8g2.drawBox(27, 16, 7, 7);
        break;
    }
  }

  else {
    for (byte track = 0; track < TRACKS; ++track) {
      byte AB_State = /*step_seq_ab[track]*/bank[currentBank].pattern[currentPattern].AB_State[track];
      byte e1, e2, e3;

      if (track == 0 || track == 4 || track == 8)
        e1 = (AB_State == 12) ? 2 : 0;

      else if (track == 1 || track == 5 || track == 9)
        e1 = (AB_State == 12) ? 11 : 9;

      else if (track == 2 || track == 6 || track == 10)
        e1 = (AB_State == 12) ? 20 : 18;

      else
        e1 = (AB_State == 12) ? 29 : 27;

      if (track < 4)
        e2 = (AB_State == 12) ? 36 : 34;

      else if (track < 8)
        e2 = (AB_State == 12) ? 27 : 25;

      else
        e2 = (AB_State == 12) ? 18 : 16;

      e3 = (AB_State == 12) ? 3 : 7;

      u8g2.drawBox(e1, e2, e3, e3);
    }
  }
}


void drawBloc_selected_Track(byte px_x, byte px_y, bool listening) {
  u8g2.drawFrame(px_x/*37*/, px_y/*14*/, 19, 26);

  u8g2.setFont(triggerDude_small_font);
  u8g2.setCursor(px_x + 5, px_y + 9);
  u8g2.print("TR");
  byte trackDisplay = selected_Track + 1;
  byte x = (listening) ? bank[currentBank].pattern[currentPattern].AB_State[selected_Track] : step_seq_ab[selected_Track];
  (trackDisplay < 10) ?
  (x == 12) ? u8g2.setCursor(px_x + 5, px_y + 19) : u8g2.setCursor(px_x + 8, px_y + 19) :
  (x == 12) ? u8g2.setCursor(px_x + 2, px_y + 19) : u8g2.setCursor(px_x + 5, px_y + 19);

  u8g2.print(selected_Track + 1);
  if (x == 12)
    u8g2.print("B");

  u8g2.setFont(triggerDude_big_font);
}


void draw_CLEAR_DELETE_UI() {
  if (update_screen_POTS_DIAL_L || update_screen_POTS_DIAL_R) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    u8g2.setFont(triggerDude_small_font);
    for (byte y = 20, z = 0; y < 50; y += 10) {
      if (clear_trigsOrPitch_cursor == z) {
        u8g2.drawBox(13, y - 4, 30, 7);
        setFontMode_BG(0);
      }
      u8g2.setCursor(15, y + 2);
      u8g2.print(clear_trigsOrPitch_param[z]);
      if (clear_trigsOrPitch_cursor == z)
        setFontMode_BG(1);


      if (clear_SelectTrack_Cursor == z) {
        u8g2.drawBox(76, y - 4, 30, 7);
        setFontMode_BG(0);
      }
      u8g2.setCursor(78, y + 2);
      u8g2.print(clear_SelectTrack_param[z]);
      if (clear_SelectTrack_Cursor == z)
        setFontMode_BG(1);
      z++;
    }


    if (!update_screen_INIT) {
      if (update_screen_POTS_DIAL_L)
        u8g2.updateDisplayArea(1, 2, 6, 4);

      if (update_screen_POTS_DIAL_R)
        u8g2.updateDisplayArea(8, 2, 6, 4);
    }

    update_screen_POTS_DIAL_L = false;
    update_screen_POTS_DIAL_R = false;
  }


  if (update_screen_PADS) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    if (Trigs_Undo_enable || Pitch_Undo_enable)
      display_enc2_click(100, "UNDO");

    update_screen_PADS = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(12, 7, 5, 1);
  }
}


void display_songMode_Grid() {
  byte boxId = 0;
  bool isBeat = (global_Step_Counter16 == 0 || global_Step_Counter16 == 4 || global_Step_Counter16 == 8 || global_Step_Counter16 == 12);

  for (byte y = 14; y < 32; y += 16) {
    for (byte x = 2; x < 121; x += 16) {
      if (boxId == songMode_current_Step_pot) {
        if (boxId == songMode_current_Step_PLAYING) {
          if (isBeat) {
            u8g2.drawBox(x, y, 14, 14);
            setFontMode_BG(0);
          }
          else
            setFontMode_BG(1);
        }
        else {
          u8g2.drawBox(x, y, 14, 14);
          setFontMode_BG(0);
        }
      }
      else {
        if (boxId == songMode_current_Step_PLAYING) {
          if (isBeat)
            u8g2.drawFrame(x, y, 14, 14);
        }
        else
          u8g2.drawFrame(x, y, 14, 14);
      }

      u8g2.setFont(triggerDude_small_font);
      u8g2.setCursor(x + 5, y + 10);
      byte stepPattern = bank[currentBank].songPatterns[boxId];

      if (stepPattern == 255)
        u8g2.print("-");
      else {
        if (stepPattern > 8)
          u8g2.setCursor(x + 2, y + 10);

        u8g2.print(stepPattern + 1);
      }

      if (boxId == songMode_current_Step_pot)
        setFontMode_BG(1);

      boxId++;
      if (boxId == 16) boxId = 0;
    }
  }
}


void draw_SHIFT_UI() {
  u8g2.drawHLine(0, 34, 128);
  u8g2.drawHLine(0, 35, 128);
  u8g2.drawVLine(63, 30, 9); // 0

  int8_t s = -(bank[currentBank].pattern[currentPattern].shiftValue[selected_Track]);
  u8g2.setFont(u8g2_font_pressstart2p_8u);
  if (s < 0) {
    u8g2.setCursor(53, 26);
  }
  else if (s < 10) {
    u8g2.setCursor(61, 26);
  }
  else {
    u8g2.setCursor(56, 26);
  }
  u8g2.print(s);

  for (int i = (63 - (get_pattern_Length(4) / 2)); i <= (63 + (get_pattern_Length(4) / 2)); i += 4 ) {
    u8g2.drawVLine(i, 32, 5);
  }
  u8g2.drawVLine((63 + s), 28, 13); // curseur de shift

  if (s != 0)
    display_enc1_click("RESET");
}


void draw_3D_box(byte pix_x, byte pix_y, byte box_w, byte box_h, const char* text, byte data_box_w) {
  // title box 3D
  u8g2.drawBox(pix_x, (pix_y + 2), box_w, box_h);
  u8g2.drawHLine((pix_x + 1), (pix_y), box_w); // ligne ombre du haut ( __ )
  u8g2.drawVLine((pix_x + (box_w + 1)), (pix_y), box_h); // ligne ombre de droite ( | )
  u8g2.drawPixel(pix_x, (pix_y + 1)); // pixel ombre "3D" haut gauche
  u8g2.drawPixel((pix_x + box_w), (pix_y + box_h)); // pixel ombre "3D" bas droite

  // title
  u8g2.setCursor((pix_x + 3), (pix_y + 12));
  u8g2.setDrawColor(0);
  u8g2.setFont(triggerDude_small_font);
  u8g2.print(text);
  u8g2.setDrawColor(1);

  // data box
  byte x_offset = pix_x + box_w + 2;
  u8g2.drawHLine(x_offset, (pix_y), data_box_w); // ligne du haut
  u8g2.drawHLine(x_offset, (pix_y + box_h - 1), data_box_w); // ligne du bas
  u8g2.drawVLine((x_offset + data_box_w - 1), (pix_y + 1), box_h - 2); // ligne de droite

  // data
  u8g2.setCursor((x_offset + 4), (pix_y + 10));
}


void drawBloc_CvTracks(byte pix_x, byte pix_y) {
  draw_3D_box(pix_x, pix_y, 15, 14, "CV", 52);

  byte cvOutCh_1 = bank[currentBank].pattern[currentPattern].cvOut_Tracks[0];
  if (cvOutCh_1 == 12) {
    u8g2.print("-");
  }
  else {
    if (cvOutCh_1 == 13 || cvOutCh_1 == 14) {
      u8g2.setCursor((pix_x + (/*box_w*/ 18)), (pix_y + 10));
      (cvOutCh_1 == 13) ? u8g2.print("L1") : u8g2.print("L2");
    }
    else {
      cvOutCh_1++;
      (cvOutCh_1 < 10) ? u8g2.setCursor((pix_x + 21), (pix_y + 10)) : u8g2.setCursor((pix_x + (/*box_w*/18)), (pix_y + 10));
      u8g2.print(cvOutCh_1);
    }
  }

  u8g2.drawVLine((pix_x + 29), (pix_y + 1), 12);
  byte cvOutCh_2 = bank[currentBank].pattern[currentPattern].cvOut_Tracks[1];
  if (cvOutCh_2 == 12) {
    u8g2.setCursor((pix_x + 34), (pix_y + 10));
    u8g2.print("-");
  }
  else {
    if (cvOutCh_2 == 13 || cvOutCh_2 == 14) {
      u8g2.setCursor((pix_x + 31), (pix_y + 10));
      (cvOutCh_2 == 13) ? u8g2.print("L1") : u8g2.print("L2");
    }
    else {
      cvOutCh_2++;
      (cvOutCh_2 < 10) ? u8g2.setCursor((pix_x + 34), (pix_y + 10)) : u8g2.setCursor((pix_x + 31), (pix_y + 10));
      u8g2.print(cvOutCh_2);
    }
  }

  u8g2.drawVLine((pix_x + 42), (pix_y + 1), 12);
  byte cvOutCh_3 = bank[currentBank].pattern[currentPattern].cvOut_Tracks[2];
  if (cvOutCh_3 == 12) {
    u8g2.setCursor((pix_x + 47), (pix_y + 10));
    u8g2.print("-");
  }
  else {
    if (cvOutCh_3 == 13 || cvOutCh_3 == 14) {
      u8g2.setCursor((pix_x + 44), (pix_y + 10));
      (cvOutCh_3 == 13) ? u8g2.print("L1") : u8g2.print("L2");
    }
    else {
      cvOutCh_3++;
      (cvOutCh_3 < 10) ? u8g2.setCursor((pix_x + 47), (pix_y + 10)) : u8g2.setCursor((pix_x + 44), (pix_y + 10));
      u8g2.print(cvOutCh_3);
    }
  }

  u8g2.drawVLine((pix_x + 55), (pix_y + 1), 12);  // 59 - 14
  byte cvOutCh_4 = bank[currentBank].pattern[currentPattern].cvOut_Tracks[3];

  if (cvOutCh_4 == 12) {
    u8g2.setCursor((pix_x + 60), (pix_y + 10));
    u8g2.print("-");
  }
  else {
    if (cvOutCh_4 == 13 || cvOutCh_4 == 14) {
      u8g2.setCursor((pix_x + 57), (pix_y + 10));
      (cvOutCh_4 == 13) ? u8g2.print("L1") : u8g2.print("L2");
    }
    else {
      cvOutCh_4++;
      (cvOutCh_4 < 10) ? u8g2.setCursor((pix_x + 60), (pix_y + 10)) : u8g2.setCursor((pix_x + 57), (pix_y + 10));
      u8g2.print(cvOutCh_4);
    }
  }
}


void drawBloc_chromaSettings_CVOutSelect() {
  byte cvOutCh_1 = bank[currentBank].pattern[currentPattern].cvOut_Tracks[0];
  byte cvOutCh_2 = bank[currentBank].pattern[currentPattern].cvOut_Tracks[1];
  byte cvOutCh_3 = bank[currentBank].pattern[currentPattern].cvOut_Tracks[2];
  byte cvOutCh_4 = bank[currentBank].pattern[currentPattern].cvOut_Tracks[3];

  u8g2.drawFrame(6, 14, 29, 29);
  u8g2.drawHLine(7, 28, 27);
  u8g2.drawVLine(20, 15, 27);

  u8g2.setFont(triggerDude_small_font);

  if (cvOutCh_1 == 12) {
    u8g2.setCursor(11, 24);
    u8g2.print("-");
  }
  else {
    if (cvOutCh_1 == 13 || cvOutCh_1 == 14) {
      u8g2.setCursor(11, 24);
      (cvOutCh_1 == 13) ? u8g2.print("L1") : u8g2.print("L2");
    }
    else {
      cvOutCh_1++;
      (cvOutCh_1 < 10) ? u8g2.setCursor(11, 24) : u8g2.setCursor(8, 24);
      u8g2.print(cvOutCh_1);
    }
  }

  if (cvOutCh_2 == 12) {
    u8g2.setCursor(25, 24);
    u8g2.print("-");
  }
  else {
    if (cvOutCh_2 == 13 || cvOutCh_2 == 14) {
      u8g2.setCursor(25, 24);
      (cvOutCh_2 == 13) ? u8g2.print("L1") : u8g2.print("L2");
    }
    else {
      cvOutCh_2++;
      (cvOutCh_2 < 10) ? u8g2.setCursor(25, 24) : u8g2.setCursor(22, 24);
      u8g2.print(cvOutCh_2);
    }
  }

  if (cvOutCh_3 == 12) {
    u8g2.setCursor(11, 38);
    u8g2.print("-");
  }
  else {
    if (cvOutCh_3 == 13 || cvOutCh_3 == 14) {
      u8g2.setCursor(11, 38);
      (cvOutCh_3 == 13) ? u8g2.print("L1") : u8g2.print("L2");
    }
    else {
      cvOutCh_3++;
      (cvOutCh_3 < 10) ? u8g2.setCursor(11, 38) : u8g2.setCursor(8, 38);
      u8g2.print(cvOutCh_3);
    }
  }

  if (cvOutCh_4 == 12) {
    u8g2.setCursor(25, 38);
    u8g2.print("-");
  }
  else {
    if (cvOutCh_4 == 13 || cvOutCh_4 == 14) {
      u8g2.setCursor(25, 38);
      (cvOutCh_4 == 13) ? u8g2.print("L1") : u8g2.print("L2");
    }
    else {
      cvOutCh_4++;
      (cvOutCh_4 < 10) ? u8g2.setCursor(25, 38) : u8g2.setCursor(22, 38);
      u8g2.print(cvOutCh_4);
    }
  }
}


void drawBloc_SeqPage(byte pix_x, byte pix_y) {
  draw_3D_box(pix_x, pix_y, 25, 14, "PAGE", 42);

  byte stepSeq_pageDisplay = stepSeq_page + 1;
  makeSpaceBefore(stepSeq_pageDisplay);
  u8g2.print(stepSeq_pageDisplay);
  u8g2.print("-");
  byte page_lastStep = stepSeq_pageDisplay + 7;
  u8g2.print(page_lastStep);
}


void display_patternEdit_things() {
  u8g2.setCursor(0, 34);
  u8g2.setFont(triggerDude_small_font);

  if (!patternToEdit_readyToAction) {
    if (patternSelected == 255) {
      u8g2.print("SELECT PATTERN");
      display_enc1_click("EXIT");
    }

    else {
      u8g2.print("PATTERN ");
      u8g2.print(patternSelected + 1);
      u8g2.print(" SELECTED");

      display_enc1_click("EXIT");
      display_enc2_dial(89, "ACTION", 1);

      switch (bank_pattern_EditAction_Cursor) {
        case 0 : // RELOAD
          display_enc2_click(89, bank_pattern_EditAction[bank_pattern_EditAction_Cursor]);
          break;

        case 1 : // SAVE
          display_enc2_click(99, bank_pattern_EditAction[bank_pattern_EditAction_Cursor]);
          break;

        case 2 : // COPY
          display_enc2_click(99, bank_pattern_EditAction[bank_pattern_EditAction_Cursor]);
          break;

        case 3 : // ERASE
          display_enc2_click(94, bank_pattern_EditAction[bank_pattern_EditAction_Cursor]);
          break;
      }
    }
  }

  else {
    if (bank_pattern_EditAction_Cursor == 0) {
      if (!saving_pattern) {
        u8g2.print("RELOAD PATTERN ?");
        display_enc1_click("CANCEL");
        display_enc2_click(102, "YES");
      }

    }

    else if (bank_pattern_EditAction_Cursor == 1) {
      if (!saving_pattern) {
        u8g2.print("SAVE PATTERN ?");
        display_enc1_click("CANCEL");
        display_enc2_click(102, "YES");
      }
      else {
        u8g2.print("SAVING...");
        saving_pattern = false;
      }
    }

    else if (bank_pattern_EditAction_Cursor == 2) {
      if (patternDestination == 255) {
        u8g2.print("SELECT DESTINATION");
      }
      else {
        u8g2.print(patternSelected + 1);
        u8g2.print(" > ");
        u8g2.print(patternDestination + 1);

        display_enc1_click("CANCEL");
        display_enc2_click(92, "PASTE");
      }
    }
    else if (bank_pattern_EditAction_Cursor == 3) {
      //display_enc2_dial(89, "Erase ?", 1);
      u8g2.print("ERASE PATTERN ?");
      display_enc1_click("CANCEL");
      display_enc2_click(102, "YES");
    }
  }
}


void display_bankEdit_things() {
  u8g2.setCursor(0, 34);
  u8g2.setFont(triggerDude_small_font);

  if (!bankToEdit_readyToAction) {
    if (bankSelected == 255) {
      u8g2.print("SELECT BANK");
    }
    else {
      u8g2.print("BANK ");
      u8g2.print(bankSelected + 1);
      u8g2.print(" SELECTED");

      display_enc2_dial(89, "ACTION", 1);

      switch (bank_pattern_EditAction_Cursor) {
        case 0 :
          display_enc2_click(89, bank_pattern_EditAction[bank_pattern_EditAction_Cursor]);
          break;
        case 1 :
          display_enc2_click(99, bank_pattern_EditAction[bank_pattern_EditAction_Cursor]);
          break;
        case 2 :
          display_enc2_click(99, bank_pattern_EditAction[bank_pattern_EditAction_Cursor]);
          break;
        case 3 :
          display_enc2_click(94, bank_pattern_EditAction[bank_pattern_EditAction_Cursor]);
          break;
      }
    }
  }

  else {
    if (bank_pattern_EditAction_Cursor == 0) {
      u8g2.print("RELOAD BANK ?");
      display_enc2_click(102, "YES");
    }
    else if (bank_pattern_EditAction_Cursor == 1) {
      u8g2.print("SAVE BANK ?");
      display_enc2_click(102, "YES");
    }
    else if (bank_pattern_EditAction_Cursor == 2) {
      if (bankDestination == 255) {
        u8g2.print("SELECT DESTINATION");
      }
      else {
        u8g2.print(bankSelected + 1);
        u8g2.print(" > ");
        u8g2.print(bankDestination + 1);

        //display_enc2_dial(89, "Action", 1);
        display_enc2_click(84, "PASTE");
      }
    }
    else if (bank_pattern_EditAction_Cursor == 3) {
      //display_enc2_dial(89, "Erase ?", 1);
      u8g2.print("ERASE BANK ?");
      display_enc2_click(102, "YES");
    }
  }
}


void makeSpaceBefore(byte elem) {
  if (elem < 10) u8g2.print("  ");
  else if (elem < 100) u8g2.print(" ");
}

void makeSpaceBeforeBpm(unsigned int elem) {
  if (elem < 10) u8g2.print("  ");
  else if (elem < 100) u8g2.print(" ");
}


void displayChromaMode_title() {
  u8g2.setCursor(71, 18);

  u8g2.drawBox(68, 12, 59, 8);
  setFontMode_BG(0);
  u8g2.setFont(triggerDude_small_font);
  u8g2.print(chromaMode_UIs[chromaMode_cursor]);
  setFontMode_BG(1);
}


void displayOctaveValue() {
  u8g2.setCursor(0, 24);
  u8g2.setFont(triggerDude_big_font);
  u8g2.print("Octave");

  (octaveValue < 24) ? u8g2.setCursor(10, 40) : u8g2.setCursor(15, 40);

  u8g2.setFont(u8g2_font_pressstart2p_8u);
  switch (octaveValue) {
    case 0:
      u8g2.print("-2"); //C0
      break;

    case 12:
      u8g2.print("-1"); //C1
      break;

    case 24:
      u8g2.print("0"); //C2 __ default
      break;

    case 36:
      u8g2.print("1"); //C3
      break;

    case 48:
      u8g2.print("2"); //C4
      break;

    case 60:
      u8g2.print("3"); //C5
      break;

    case 72:
      u8g2.print("4"); //C5
      break;

    case 84:
      u8g2.print("5"); //C5
      break;
  }
}


void displayTransposeValue() {
  u8g2.setCursor(0, 24);
  u8g2.setFont(triggerDude_big_font);
  u8g2.print("Octave");

  int8_t tVal = bank[currentBank].pattern[currentPattern].transposeValue[selected_Track + bank[currentBank].pattern[currentPattern].AB_State[selected_Track]];
  (tVal < 0) ? u8g2.setCursor(10, 40) : u8g2.setCursor(15, 40);

  u8g2.setFont(u8g2_font_pressstart2p_8u);

  u8g2.print(tVal);
}


void display_global_Step_4PPQN() {
  u8g2.setFont(triggerDude_big_font);
  u8g2.setCursor(84, 7);
  byte current_Step_Display = (displayFirstStep_1) ? 1 : get_current_Step_Display_4PPQN(1);
  makeSpaceBefore(current_Step_Display);
  u8g2.print(current_Step_Display);

  u8g2.setCursor(104, 7);
  u8g2.print("/");
  //uint16_t pattern_LengthDisplay = (bank[currentBank].pattern[currentPattern].tracksRetrig) ? get_pattern_Length(4) : get_track_Length(selected_Track, 4);
  uint16_t pattern_LengthDisplay = get_pattern_Length(4);
  u8g2.print(pattern_LengthDisplay);
  makeSpaceBefore(pattern_LengthDisplay);
}


void display_CurrentBank(byte pix_x, byte pix_y) {     //  0 - 12 - 27 - 14
  byte currentBankDisplay = get_currentBank_Display(1);
  draw_3D_box(pix_x, pix_y, 27, 14, "BANK", 13);
  u8g2.print(currentBankDisplay);
}


void display_CurrentPattern(byte pix_x, byte pix_y) {
  byte currentPatternDisplay = get_currentPattern_Display(1);
  draw_3D_box(pix_x, pix_y, 42, 14, "PATTERN", 13);
  u8g2.print(currentPatternDisplay);
}


void draw_pitch_UI() {
  if (stepSeq_clickedStep == 255) {
    u8g2.setCursor(2, 24);
    u8g2.print("PRESS");
    u8g2.setCursor(2, 34);
    u8g2.print("A STEP");
  }
  else {
    u8g2.setCursor(6, 24);
    u8g2.print("NOTE");
    u8g2.setCursor(6, 40);
    u8g2.setFont(u8g2_font_pressstart2p_8u);
    u8g2.print(notesNames[pitchPointer_Pot/*pitchPointer_Pad*/]);
    u8g2.setFont(triggerDude_big_font);
  }
}


void draw_glide_UI() {
  byte currentBankDisplay = get_currentBank_Display(0);
  byte currentPatternDisplay = get_currentPattern_Display(0);
  u8g2.drawHLine(0, 9, 128);

  u8g2.setCursor(6, 24);
  u8g2.print("STEPS");
  u8g2.setFont(u8g2_font_pressstart2p_8u);
  u8g2.setCursor(6, 40);
  u8g2.print(bank[currentBankDisplay].pattern[currentPatternDisplay].glide_steps[selected_Track]);
  u8g2.setFont(triggerDude_big_font);
}


void draw_Chroma_GLIDE_UI() {
  byte currentBankDisplay = get_currentBank_Display(0);
  byte currentPatternDisplay = get_currentPattern_Display(0);

  u8g2.drawHLine(0, 9, 128);

  // MODE
  u8g2.setFont(triggerDude_small_font);
  u8g2.setCursor(3, 24);
  u8g2.print("MODE:");
  if (GLIDE_paramSelection_cursor == 0) {
    u8g2.drawFrame(0, 15, 64, 13);
    if (GLIDE_paramSelection_active) {
      u8g2.drawBox(32, 17, 30, 9);
      setFontMode_BG(0);
    }
  }
  u8g2.setCursor(34, 24);
  u8g2.print(chromaMode_Glide_params_labels[bank[currentBankDisplay].pattern[currentPatternDisplay].glide_mode[selected_Track]]);
  if (GLIDE_paramSelection_cursor == 0)
    setFontMode_BG(1);

  // TIME
  u8g2.setCursor(3, 36);
  u8g2.print("TIME:");
  if (GLIDE_paramSelection_cursor == 1) {
    u8g2.drawFrame(0, 27, 64, 13);
    if (GLIDE_paramSelection_active) {
      u8g2.drawBox(32, 29, 30, 9);
      setFontMode_BG(0);
    }
  }
  u8g2.setCursor(44, 36);
  u8g2.print(bank[currentBankDisplay].pattern[currentPatternDisplay].glide_steps[selected_Track]);
  //draw_glide_UI
  u8g2.print("/");
  u8g2.print(bank[currentBankDisplay].pattern[currentPatternDisplay].glide_active);  // test

  if (GLIDE_paramSelection_cursor == 1)
    setFontMode_BG(1);
}


void draw_arp_UI() {
  if (chromaMode_Arp_params == 1) {
    u8g2.drawBox(4, 18, 64, 8);
    setFontMode_BG(0);
  }
  u8g2.setCursor(6, 24);
  u8g2.print(chromaMode_Arp_params_labels[0]); // RATE
  u8g2.print(" : ");
  u8g2.print(arp_Rate[arp_Rate_Cursor]);
  setFontMode_BG(1);


  if (chromaMode_Arp_params == 2) {
    u8g2.drawBox(4, 28, 64, 8);
    setFontMode_BG(0);
  }
  u8g2.setCursor(6, 34);
  u8g2.print(chromaMode_Arp_params_labels[1]); // STYLE
  u8g2.print(" : ");
  u8g2.print(arp_Style[arp_Style_Cursor]);
  if (chromaMode_Arp_params == 2)
    setFontMode_BG(1);
}


void display_curTrack_FILLRATE() {
  u8g2.setFont(u8g2_font_pressstart2p_8u);
  u8g2.setCursor(52, 24);
  u8g2.print("RATE");
  u8g2.setCursor(52, 38);
  switch (bank[currentBank].pattern[currentPattern].fill_rate[selected_Track]) {
    case 16:
      u8g2.print("1/16");
      break;

    case 12:
      u8g2.print("1/12");
      break;

    case 8:
      u8g2.print("1/8");
      break;

    case 6:
      u8g2.print("1/6");
      break;

    case 4:
      u8g2.print("1/4");
      break;

    case 3:
      u8g2.print("1/3");
      break;

    case 2 :
      u8g2.print("1/2");
      break;
  }

  u8g2.setFont(triggerDude_small_font);
}


void print_Dial_param_title(byte x, byte y, const char* title) {
  u8g2.setFont(triggerDude_small_font);
  u8g2.setCursor(x, y);
  u8g2.print(title);
}


void display_enc1_dial(const char* title, bool isAbove) {
  int y = (isAbove) ? 55 : 63;
  u8g2.setFont(u8g2_font_unifont_t_symbols);
  u8g2.drawGlyph(0, (y + 1), 0x21bb);
  print_Dial_param_title(10, y, title);
}


void display_enc1_click(const char* title) {
  u8g2.setFont(u8g2_font_unifont_t_symbols);
  u8g2.drawGlyph(0, 65, 0x25be);
  print_Dial_param_title(10, 63, title);
}


void display_enc2_dial(byte x, const char* title, bool isAbove) {
  byte y = (isAbove) ? 55 : 63;
  u8g2.setFont(u8g2_font_unifont_t_symbols);
  u8g2.drawGlyph(120, (y + 1), 0x21bb);
  print_Dial_param_title(x, y, title);
}


void display_enc2_click(byte x, const char* title) {
  u8g2.setFont(u8g2_font_unifont_t_symbols);
  u8g2.drawGlyph(120, 65, 0x25be);
  print_Dial_param_title(x, 63, title);
}



/*

  void drawPage_TEMPLATE() {
  if (update_screen_INIT) {
    set_default_UI_header("SCREEN_NAME");
  }


  if (update_screen_TOP) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    display_global_Step_4PPQN();

    update_screen_TOP = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(10, 0, 6, 1);
  }


  if (update_screen_PADS) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    update_screen_PADS = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(13, 4, 2, 1);
  }


  if (update_screen_ALT_BTN) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    update_screen_ALT_BTN = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(11, 6, 5, 2);
  }


  if (update_screen_POTS_DIAL_L) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    update_screen_POTS_DIAL_L = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(5, 6, 2, 1);
  }


  if (update_screen_POTS_CLK_L) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    update_screen_POTS_CLK_L = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(0, 7, 5, 1);
  }


  if (update_screen_POTS_DIAL_R) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    update_screen_POTS_DIAL_R = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(12, 7, 4, 1);
  }


  if (update_screen_POTS_CLK_R) {
    if (!update_screen_INIT)
      u8g2.clearBuffer();

    update_screen_POTS_CLK_R = false;
    if (!update_screen_INIT)
      u8g2.updateDisplayArea(11, 7, 5, 1);
  }


  if (update_screen_INIT) {
    u8g2.sendBuffer();
    update_screen_INIT = false;
  }

  }

*/
