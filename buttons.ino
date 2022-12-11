void reset_modes_on_screenChange(/*byte currentScreen*/) {
  //u8g2.clearBuffer();
  ALT_pushed = false;
  recMode = 0;
  pitchMode = 0;
  MICROTIMING = 0;
  glideMode = 0;
  Trigs_Undo_enable = 0;
  Pitch_Undo_enable = 0;
  reset_ChromaMode_UI();
  update_padLeds = true;
  canResetScreen = false;

  //if (currentScreen != screenConfig)
  update_screen_INIT = true;
}


void detect_remoteBtnsPush() {
  // BACK BUTTON
  bool remoteBtn_Back_State = btnsIn_BitVal[1];
  if (remoteBtn_Back_State != last_remoteBtn_Back_State && ((millis() - last_btn_DebounceTime_millis) > DEBOUNCE_BTNS_MILLIS)) {
    last_btn_DebounceTime_millis = millis();
    if (remoteBtn_Back_State == 1) {

      if (ALT_pushed && screenConfig != 12) { // on affiche PITCH SEQ SCREEN
        screenConfig = 12;
        reset_modes_on_screenChange();
        pitchMode = 1;
        stepSeq_clickedStep = 255;
      }

      else if (!ALT_pushed) {
        global_Step_4PPQN = get_pattern_Length(4) - 1;
        global_Step_16PPQN = get_pattern_Length(16) - 1;

        for (byte track = 0; track < TRACKS; ++track) {
          track_Step_16PPQN[track] = get_track_Length(track, 16) - 1;
        }

        //tick = 31;
        global_Step_Counter16 = 15; //global_Step_Counter16 = 0;

        if (!PLAYING) {
          displayFirstStep_1 = 1;
          shiftOutTrigs(12); // produit un trig sur la sortie CLOCK OUT
        }
        else
          displayFirstStep_1 = 0;
      }

      update_screen_TOP = true;
      update_padLeds = true;
    }

    last_remoteBtn_Back_State = remoteBtn_Back_State;
  }

  // PLAY BUTTON
  bool remoteBtn_Play_State = btnsIn_BitVal[0];
  if (remoteBtn_Play_State != last_remoteBtn_Play_State && ((millis() - last_btn_DebounceTime_millis) > DEBOUNCE_BTNS_MILLIS)) {
    last_btn_DebounceTime_millis = millis();
    if (remoteBtn_Play_State == 1) {

      if (ALT_pushed && (screenConfig != 7)) { // on affiche SETTINGS SCREEN
        reset_modes_on_screenChange();
        screenConfig = 7;
        //recMode = 0;
        //canResetScreen = false;
        //update_screen_INIT = true;
      }

      else if (!ALT_pushed) {
        if (SYNC_mode_internal) {
          PLAY_pushed = !PLAY_pushed;
          PLAYING = !PLAYING;
        }

        else {
          PLAY_pushed = !PLAY_pushed;
          if (PLAYING)
            PLAYING = 0;
        }
      }

      //update_screen = true;
      update_padLeds = true;
    }

    last_remoteBtn_Play_State = remoteBtn_Play_State;
  }

  // REC BUTTON
  bool remoteBtn_Rec_State = btnsIn_BitVal[3];
  if (remoteBtn_Rec_State != last_remoteBtn_Rec_State && ((millis() - last_btn_DebounceTime_millis) > DEBOUNCE_BTNS_MILLIS)) {
    last_btn_DebounceTime_millis = millis();
    if (remoteBtn_Rec_State == 1) {
      if (ALT_pushed && screenConfig != 10) { // on affiche STEP SEQ SCREEN
        screenConfig = 10;
        recMode = 0;
        canResetScreen = false;
        update_screen_INIT = true;
      }

      else if (!ALT_pushed && screenConfig == 10) {
        MICROTIMING = !MICROTIMING;
        stepSeq_clickedStep = 255;
      }

      else if ((!ALT_pushed && screenConfig == 1) || screenConfig == 33 || screenConfig == 5) {
        recMode = !recMode;
        if (recMode && screenConfig == 5) {
          fillDataRecording_startStep = get_track_step_TRIGS(selected_Track, 0);
        }
      }

      update_padLeds = true;
    }

    last_remoteBtn_Rec_State = remoteBtn_Rec_State;
  }

  // ALT BUTTON
  bool remoteBtn_Alt_State = btnsIn_BitVal[2];

  if (remoteBtn_Alt_State != last_remoteBtn_Alt_State && ((millis() - last_btn_DebounceTime_millis) > DEBOUNCE_BTNS_MILLIS)) {
    last_btn_DebounceTime_millis = millis();

    if (remoteBtn_Alt_State == 1) {
      canResetScreen = true;
      ALT_pushed = true;
    }

    else {
      if (canResetScreen && screenConfig != 1)
        reset_display_Screen();
      ALT_pushed = false;
      digitalWrite(altBtnLed, LOW);
      update_padLeds = true;
    }

    update_screen_ALT_BTN = true;
    last_remoteBtn_Alt_State = remoteBtn_Alt_State;
  }

  if (remoteBtn_Alt_State == 1) {
    ALT_pushed = true;
    digitalWrite(altBtnLed, HIGH);
  }

  /*else {
    ALT_pushed = false;
    digitalWrite(altBtnLed, LOW);
    }*/

}





void catch_PatternBank_OnOff() {
  bool patternBank_ButtonState = digitalRead(patternBank_Button);
  if (patternBank_ButtonState != last_patternBank_ButtonState && millis() - lastDebounceTime_millis > DEBOUNCE_BTNS_MILLIS) {
    lastDebounceTime_millis = millis();
    last_patternBank_ButtonState = patternBank_ButtonState;

    if (patternBank_ButtonState == HIGH) {
      if (ALT_pushed) {
        if (screenConfig != 33) // Pour l'instant BANK sert à lancer CHROMA MODE
          screenConfig = 33;

        else
          reset_display_Screen();
      }

      else {
        if (screenConfig != 2)
          screenConfig = 2;

        else
          reset_display_Screen();
      }
    }

    reset_modes_on_screenChange(/*2*/);
  }
}


void catch_AbLength_OnOff() {
  bool AbLength_ButtonState = digitalRead(AbLength_Button);
  if (AbLength_ButtonState != last_AbLength_ButtonState && millis() - lastDebounceTime_millis > DEBOUNCE_BTNS_MILLIS) {
    lastDebounceTime_millis = millis();
    last_AbLength_ButtonState = AbLength_ButtonState;

    if (AbLength_ButtonState == HIGH) {
      if (ALT_pushed) {
        if (screenConfig != 9) {
          screenConfig = 9;
        }
        else reset_display_Screen();
      }
      else {
        if (screenConfig != 3) {
          screenConfig = 3;
        }
        else reset_display_Screen();
      }
    }

    reset_modes_on_screenChange(/*2*/);
  }
}


void catch_MuteSolo_OnOff() {
  bool muteSolo_ButtonState = digitalRead(muteSolo_Button);
  if (muteSolo_ButtonState != last_muteSolo_ButtonState && millis() - lastDebounceTime_millis > DEBOUNCE_BTNS_MILLIS) {
    lastDebounceTime_millis = millis();
    last_muteSolo_ButtonState = muteSolo_ButtonState;

    if (muteSolo_ButtonState == HIGH) {
      if (ALT_pushed) {
        if (screenConfig != 44) {
          screenConfig = 44;
        }
        else reset_display_Screen();
      }
      else {
        if (screenConfig != 4) {
          screenConfig = 4;
        }
        else reset_display_Screen();
      }
    }

    reset_modes_on_screenChange(/*2*/);
  }
}


void catch_FillShift_OnOff() {
  bool fillShift_ButtonState = digitalRead(fillShift_Button);
  if (fillShift_ButtonState != last_fillShift_ButtonState && millis() - lastDebounceTime_millis > DEBOUNCE_BTNS_MILLIS) {
    lastDebounceTime_millis = millis();
    last_fillShift_ButtonState = fillShift_ButtonState;

    if (fillShift_ButtonState == HIGH) {
      if (ALT_pushed) {
        if (screenConfig != 55) {
          screenConfig = 55;
        }
        else reset_display_Screen();
      }
      else {
        if (screenConfig != 5) {
          screenConfig = 5;
        }
        else reset_display_Screen();
      }
    }

    reset_modes_on_screenChange(/*2*/);
  }
}


void catch_DeleteClear_OnOff() {
  bool deleteClear_ButtonState = digitalRead(deleteClear_Button);
  if (deleteClear_ButtonState != last_deleteClear_ButtonState && millis() - lastDebounceTime_millis > DEBOUNCE_BTNS_MILLIS) {
    lastDebounceTime_millis = millis();
    last_deleteClear_ButtonState = deleteClear_ButtonState;

    if (deleteClear_ButtonState == HIGH) {
      if (ALT_pushed) {
        if (screenConfig != 66) {
          screenConfig = 66;
          buffer_Sequences();
        }
        else
          reset_display_Screen();
      }

      else {
        if (screenConfig != 6) {
          screenConfig = 6;
          buffer_Sequences();
        }
        else
          reset_display_Screen();
      }
    }

    reset_modes_on_screenChange(/*2*/);
  }
}


void catch_Mode_Selection_Change() {
  catch_PatternBank_OnOff();
  catch_AbLength_OnOff();
  catch_MuteSolo_OnOff();
  catch_FillShift_OnOff();
  catch_DeleteClear_OnOff();
}
