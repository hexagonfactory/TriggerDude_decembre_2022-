void detect_set_FIRSTSTEP_Range() {
  enc2_Range_10 = !enc2_Range_10;
  enc2_clk = false;
}


void detect_set_BPM_Range() {
  enc2_Range_10 = !enc2_Range_10;
  enc2_clk = false;
}


void detect_set_LFO_gain_Range() {
  enc2_Range_10 = !enc2_Range_10;
  enc2_clk = false;
}


void detect_encoder1_Range() {
  enc1_Range_10 = !enc1_Range_10;
  enc1_clk = false;
}


void detect_encoder2_Range() {
  enc2_Range_10 = !enc2_Range_10;
  enc2_clk = false;
}


void detect_encoder1_pitch_Range() {
  pitch_Range_12 = !pitch_Range_12;
  enc1_clk = false;
}


void detect_Settings_exit() {
  reset_display_Screen();
  enc1_clk = false;
}


void detect_param_cvMod_OnOff(bool cvInNum) {
  if (CVin[!cvInNum].enable && CVin[cvInNum].dest_Cursor == CVin[!cvInNum].dest_Selected)
    return;

  else {
    CVin[cvInNum].enable = !CVin[cvInNum].enable;
    if (CVin[cvInNum].enable)
      CVin[cvInNum].dest_Selected = CVin[cvInNum].dest_Cursor;
  }

  if (CVin[cvInNum].dest_Cursor == 6 || CVin[cvInNum].dest_Cursor == 8)
    update_LFO_Rate_micros();

  songMode_Activated = 0;
  enc2_clk = false;
}


void detect_rollOnOff() {
  if (rollMode == 1) {
    screenConfig = 1;
    rollMode = 0;
  }

  else {
    screenConfig = 8;
    rollMode = 1;
    recMode = 0;
  }
  
  update_screen_INIT = true;
  update_padLeds = true;
  enc1_clk = false;
}


void detect_lockpattern_LengthOnOff() {
  if (pattern_Length_Locked == 1) {
    pattern_Length_Locked = 0;
  }

  else {
    pattern_Length_Locked = 1;
    if (ALT_pushed) {     // vérifier pourquoi ALTENABLE ???
      for (byte i = 0; i < sizeof(pattern_LengthArray) / sizeof(pattern_LengthArray[0]); ++i) {
        if (bank[currentBank].pattern[currentPattern].pattern_Length < pattern_LengthArray[i]) {
          bank[currentBank].pattern[currentPattern].pattern_Length = pattern_LengthArray[i];
          break;
        }
      }
    }
  }

  enc2_clk = false;
}

/*void detect_pitchMode_OnOff() {
  bool encoder2_Btn_State = digitalRead(encoder2_Btn);
  if (encoder2_Btn_State != last_encoder2_Btn_State) {
    if (encoder2_Btn_State == 0) {
      if (pitchMode == 0) {
        screenConfig = 12;
        pitchMode = 1;
      }
      else {
        screenConfig = 10;
        pitchMode = 0;
      }

      stepSeq_clickedStep = 255;
      MICROTIMING = 0;

      update_padLeds = true;
      //update_screen = true;
    }
  }
  last_encoder2_Btn_State = encoder2_Btn_State;
  }*/


void detect_stepSeq_params_OnOff() {
  if (screenConfig != 11) {
    screenConfig = 11;
  }

  else {
    screenConfig = 10;
  }

  //stepSeq_clickedStep = 255;
  MICROTIMING = 0;

  update_padLeds = true;
  enc2_clk = false;
}


void detect_GlideMode_OnOff_seq() {
  if (glideMode == 0) {
    screenConfig = 16;
    glideMode = 1;
    pitchMode = 0;
  }
  else {
    screenConfig = 12;
    glideMode = 0;
    pitchMode = 1;
  }

  update_padLeds = true;
  enc1_clk = false;
}


void detect_GlideMode_OnOff_chroma() {
  glideMode = !glideMode;
  update_padLeds = true;
  enc1_clk = false;
}


void detect_Track_AB_change(byte curTrack) {
  step_seq_ab[curTrack] = (step_seq_ab[curTrack] == 0) ? 12 : 0;
  update_padLeds = true;
  enc1_clk = false;
}


void detect_swapABStates() {
  for (byte track = 0; track < TRACKS; ++track) {
    bank[currentBank].pattern[currentPattern].AB_State[track] = (bank[currentBank].pattern[currentPattern].AB_State[track] == 0) ? 12 : 0;
  }

  update_padLeds = true;
  enc2_clk = false;
}


void detect_resetABStates() {
  byte x = (bank[currentBank].pattern[currentPattern].allAorB) ? 12 : 0;
  for (byte track = 0; track < TRACKS; ++track) {
    bank[currentBank].pattern[currentPattern].AB_State[track] = x;
  }
  update_padLeds = true;
  enc1_clk = false;
}


void detect_resetMuteStates() {
  for (byte track = 0; track < TRACKS; ++track) {
    //bank[currentBank].pattern[currentPattern].muteState[track] = 0;
    uint8_t byteToWrite = track / 8;
    uint8_t bitPos = track & 7;
    bank[currentBank].pattern[currentPattern].mute_solo_States[byteToWrite] &= ~masks[bitPos];
  }

  bank[currentBank].pattern[currentPattern].mute_active = 0;
  bank[currentBank].pattern[currentPattern].allMute_active = 0;

  update_padLeds = true;
  enc1_clk = false;

}


void detect_muteAll() {
  for (byte track = 0; track < TRACKS; ++track) {
    //bank[currentBank].pattern[currentPattern].muteState[track] = 1;
    uint8_t byteToWrite = track / 8;
    uint8_t bitPos = track & 7;
    bank[currentBank].pattern[currentPattern].mute_solo_States[byteToWrite] |= masks[bitPos];
  }

  bank[currentBank].pattern[currentPattern].mute_active = 1;
  bank[currentBank].pattern[currentPattern].allMute_active = 1;

  update_padLeds = true;
  enc2_clk = false;
}


void detect_resetSoloStates() {
  for (byte track = 0; track < TRACKS; ++track) {
    uint8_t byteToWrite = (track + 12) / 8;
    uint8_t bitPos = (track + 12) & 7;
    bank[currentBank].pattern[currentPattern].mute_solo_States[byteToWrite] &= ~masks[bitPos];
  }

  bank[currentBank].pattern[currentPattern].solo_active = 0;

  update_padLeds = true;
  enc1_clk = false;

}


void detect_resetFillStates() {
  for (byte track = 0; track < TRACKS; ++track) {
    uint8_t byteToWrite = track / 8;
    uint8_t bitPos = track & 7;
    bank[currentBank].pattern[currentPattern].fillState[byteToWrite] &= ~masks[bitPos];

  }
  bank[currentBank].pattern[currentPattern].fill_active = 0;

  enc2_clk = false;
}


void detect_clear_Track_Shift() {
  bank[currentBank].pattern[currentPattern].shiftValue[selected_Track] = 0;

  for (byte track = 0; track < TRACKS; ++track) {
    if (bank[currentBank].pattern[currentPattern].shiftValue[track] != 0) {
      bank[currentBank].pattern[currentPattern].shift_active = 1;
      break;
    }
    else
      bank[currentBank].pattern[currentPattern].shift_active = 0;
  }

  update_padLeds = true;
  enc1_clk = false;
}


void detect_clearAllShift() {
  for (byte track = 0; track < TRACKS; ++track) {
    bank[currentBank].pattern[currentPattern].shiftValue[track] = 0;
  }

  bank[currentBank].pattern[currentPattern].shift_active = 0;

  update_padLeds = true;
  enc2_clk = false;
}


void detect_clearPitch(byte curTrack) {
  for (byte i = 0; i < MAX_STEPS_4PPQN; ++i) {
    clearSequence(currentBank, currentPattern, curTrack, i, 1, 1);
  }
  pitchPointer_Pad = 24;
  pitchPointer_Pot = 24;

  enc1_clk = false;

}


void detect_ChromaMode_transpose_reset() {
  byte realTrack = selected_Track + bank[currentBank].pattern[currentPattern].AB_State[selected_Track];
  bank[currentBank].pattern[currentPattern].transposeValue[realTrack] = 0;

  enc1_clk = false;
}


void detect_songMode_Screen() {
  patternSelected = 255;
  screenConfig = (screenConfig == 2) ? 14 : 2;

  update_padLeds = true;
  enc2_clk = false;
}


void detect_songMode_OnOff() {
  songMode_Activated = !songMode_Activated;
  PLAY_pushed = 0;
  PLAYING = 0;
  global_Step_16PPQN = get_firstStep(16);
  global_Step_4PPQN = get_firstStep(4);
  songMode_current_Step_PLAYING = 0;

  enc1_clk = false;
}


void detect_patternEditMode_OnOff() {
  detect_patternEditMode_Reset();
  screenConfig = (screenConfig == 2) ? 13 : 2;
}


void detect_patternEditMode_Reset() {
  patternSelected = 255;
  patternDestination = 255;
  patternToEdit_readyToAction = 0;
  update_padLeds = true;
  update_screen_INIT = true;
  enc1_clk = false;
}


void detect_bankEditMode_OnOff() {
  bankSelected = 255;
  bankDestination = 255;
  bankToEdit_readyToAction = 0;
  screenConfig = (screenConfig == 22) ? 15 : 22;

  update_padLeds = true;
  update_screen_INIT = true;
  enc1_clk = false;
}


void detect_patternSelected_Edit() { // Au click on fait l'action (save / copy / erase) sur patternToEdit_readyToAction
  patternToEdit_readyToAction = 1;

  update_padLeds = true;
  enc2_clk = false;
}


void detect_bankSelected_Edit() { // Au click on fait l'action (save / copy / erase) sur bankToEdit_readyToAction
  bankToEdit_readyToAction = 1;

  update_padLeds = true;
  enc2_clk = false;
}


void detect_patternPaste() {
  bank[currentBank].pattern[patternDestination] = bank[currentBank].pattern[patternSelected];
  bank[currentBank].notEmpty_Pattern[patternDestination] = 1;
  currentPattern = patternDestination;
  patternToEdit_readyToAction = 0;
  patternSelected = 255;
  screenConfig = 13;
  update_padLeds = true;
  enc2_clk = false;
}


void detect_reload_pattern(byte zeBank, byte zePattern) {
  EEPROM.get(BANK_ADR_0, bank[zeBank].notEmpty_Pattern[zePattern]);
  EEPROM.get(patterns_addresses[zePattern], bank[zeBank].pattern[zePattern]);
  patternToEdit_readyToAction = 0;
  patternSelected = 255;
  screenConfig = 13;
  update_padLeds = true;
  enc2_clk = false;
}


void detect_bankPaste() {
  bank[bankDestination] = bank[bankSelected];
  currentBank = bankDestination;
  bankToEdit_readyToAction = 0;
  bankSelected = 255;
  screenConfig = 15;
  update_padLeds = true;
  enc2_clk = false;
}


void detect_patternErase() {
  reset_Pattern(currentBank, patternSelected);
  currentPattern = patternSelected;
  patternToEdit_readyToAction = 0;
  patternSelected = 255;
  screenConfig = 13;
  update_padLeds = true;
  enc2_clk = false;
}


void detect_bankErase() {
  resetBank(currentBank);
  currentBank = bankSelected;
  bankToEdit_readyToAction = 0;
  bankSelected = 255;
  screenConfig = 15;
  update_padLeds = true;
  enc2_clk = false;
}


void detect_save_pattern() {
  EEPROM.write(EEPROM_INIT, 1);
  EEPROM.put(BANK_ADR_0, bank[currentBank].notEmpty_Pattern);
  EEPROM.put(patterns_addresses[currentPattern], bank[currentBank].pattern[currentPattern]);
  patternToEdit_readyToAction = 0;
  patternSelected = 255;
  screenConfig = 13;
  update_padLeds = true;
  enc2_clk = false;
}

/*
  void detect_save_bank() {
  bool encoder2_Btn_State = digitalRead(encoder2_Btn);
  if (encoder2_Btn_State != last_encoder2_Btn_State) {
    if (encoder2_Btn_State == 0) {
      EEPROM.put(BANK_ADR_0, bank[currentBank]);
      bankToEdit_readyToAction = 0;
      bankSelected = 255;
      screenConfig = 22;
      update_padLeds = true;
      //update_screen = true;
    }
  }
  last_encoder2_Btn_State = encoder2_Btn_State;
  }
*/

void detect_lfo_onOff(byte lfoNum) {

  byte lfoTrack = (lfoNum == 0) ? 13 : 14;
  if (bank[currentBank].pattern[currentPattern].cvOut_Tracks[bank[currentBank].pattern[currentPattern].lfo_dest[lfoNum]] == lfoTrack) { // on met LFO sur OFF
    bank[currentBank].pattern[currentPattern].cvOut_Tracks[bank[currentBank].pattern[currentPattern].lfo_dest[lfoNum]] = 12;
    bank[currentBank].pattern[currentPattern].lfo_enable[lfoNum] = LFO_enable_State(lfoNum);
  }

  else {
    if (bank[currentBank].pattern[currentPattern].cvOut_Tracks[bank[currentBank].pattern[currentPattern].lfo_dest[lfoNum]] == 12) { // on met LFO sur ON
      bank[currentBank].pattern[currentPattern].cvOut_Tracks[bank[currentBank].pattern[currentPattern].lfo_dest[lfoNum]] = lfoTrack;
      bank[currentBank].pattern[currentPattern].lfo_enable[lfoNum] = 1;
      LFO[lfoNum].rate_micros = get_LFO_Rate_micros(lfoNum);
      LFO[lfoNum].counter = 0;

      update_LFO_Rate_micros();
    }


    else {
      // mettre ici message disant que ce cvOut est pris pas un track
    }
  }

  LFOs_enable_disable();
  enc2_clk = false;
}


void detect_reset_LFO_PHASE(byte lfoNum) {
  bank[currentBank].pattern[currentPattern].lfo_phase[lfoNum] = 0;
  enc2_clk = false;
}


void detect_undoCall() {
  if (Trigs_Undo_enable) {
    for (byte track = 0; track < TRACKS_AB; ++track) {
      for (uint16_t steps = 0; steps < MAX_STEPS_16PPQN; ++steps) {
        bank[currentBank].pattern[currentPattern].Trigs[track][steps / 8] = Trigs_Undo_buffer[track][steps / 8];
      }
    }

    Trigs_Undo_enable = 0;
  }

  if (Pitch_Undo_enable) {
    for (byte track = 0; track < TRACKS_AB; ++track) {
      for (byte steps = 0; steps < MAX_STEPS_4PPQN; ++steps) {
        bank[currentBank].pattern[currentPattern].Pitch[track][steps] =  Pitch_Undo_buffer[track][steps];
      }
    }

    Pitch_Undo_enable = 0;
  }

  for (byte track = 0; track < TRACKS_AB; ++track) {
    for (uint16_t steps = 0; steps < MAX_STEPS_16PPQN; ++steps) {
      bank[currentBank].pattern[currentPattern].Trigs[track][steps / 8] = Trigs_Undo_buffer[track][steps / 8];

      if ((bank[currentBank].pattern[currentPattern].Trigs[track][steps / 8] & masks[steps & 7]) > 0) {
        bank[currentBank].pattern[currentPattern].track_has_Trigs[track / 8] |= masks[track & 7];
        break;
      }

      else {
        if (steps == MAX_STEPS_16PPQN - 1) {
          bank[currentBank].pattern[currentPattern].track_has_Trigs[track / 8] &= ~masks[track & 7];
        }
      }
    }
  }

  enc2_clk = false;
}


void detect_GLIDE_paramSelection_onOff() {
  GLIDE_paramSelection_active = !GLIDE_paramSelection_active;
  enc1_clk = false;
}


void detect_cvOutReassign(byte cvOutput, byte track) { // fonction a modifier (enc1_clk = false; etc)
  bool encoder1_Btn_State = digitalRead(encoder1_Btn);
  bool encoder2_Btn_State = digitalRead(encoder2_Btn);

  if (encoder1_Btn_State != last_encoder1_Btn_State) {
    if (encoder1_Btn_State == 0) {
      confirmed_CvOutReassign = 0;
      trigger_cvOutReassign_process = 0;
      //update_screen = true;
    }

    last_encoder1_Btn_State = encoder1_Btn_State;
  }

  if (encoder2_Btn_State != last_encoder2_Btn_State) {
    if (encoder2_Btn_State == 0) {
      confirmed_CvOutReassign = 1;
      trigger_cvOutReassign_process = 0;
      bank[currentBank].pattern[currentPattern].cvOut_Tracks[cvOutput] = track;
      //update_screen = true;
    }

    last_encoder2_Btn_State = encoder2_Btn_State;
  }
}


void detect_ARP_params_onOff() {
  chromaMode_Arp_params = !chromaMode_Arp_params;
  enc1_clk = false;
}


void detect_ARP_latch_onOff() {
  arp_latch = !arp_latch;
  arp_latch_active = 0;
  enc2_clk = false;
}
