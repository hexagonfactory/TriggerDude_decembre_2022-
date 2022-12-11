void sequencer_play_song() {
  currentPattern = bank[currentBank].songPatterns[songMode_current_Step_PLAYING];
  if (global_Step_4PPQN == bank[currentBank].pattern[ bank[currentBank].songPatterns[songMode_current_Step_PLAYING] ].pattern_Length) {
    songMode_current_Step_PLAYING = (bank[currentBank].songPatterns[songMode_current_Step_PLAYING + 1] == 255) ? 0 : songMode_current_Step_PLAYING + 1;
    loadPattern_params();
  }
}



void manage_changeOn_FIRSTSTEP_things() {
  if (PLAYING && (global_Step_16PPQN == get_firstStep(16))) {
    if (patternChange_Quantized && patternChange_triggered) {
      currentPattern = nextPattern;
      loadPattern_params();
      patternChange_triggered = false;
    }
  }
}


byte get_currentPattern_Display(byte numToAdd) {
  byte x = ((CVin[0].enable && (CVin[0].dest_Selected == 0)) || (CVin[1].enable && (CVin[1].dest_Selected == 0))) ? modulated_currentPattern : currentPattern;
  return x + numToAdd;
}

byte get_currentBank_Display(byte numToAdd) {
  byte x = ((CVin[0].enable && (CVin[0].dest_Selected == 1)) || (CVin[1].enable && (CVin[1].dest_Selected == 1))) ? modulated_currentBank : currentBank;
  return x + numToAdd;
}

byte get_track_Step_Display_4PPQN(byte track, bool stepToAdd) {
  byte x = ((CVin[0].enable && (CVin[0].dest_Selected == 2)) || (CVin[1].enable && (CVin[1].dest_Selected == 2))) ? modulated_global_Step : track_Step_16PPQN[track] / 4;
  return x + stepToAdd;
}

byte get_current_Step_Display_4PPQN(bool stepToAdd) {
  byte x = ((CVin[0].enable && (CVin[0].dest_Selected == 2)) || (CVin[1].enable && (CVin[1].dest_Selected == 2))) ? modulated_global_Step : global_Step_4PPQN;
  return x + stepToAdd;
}

uint16_t get_pattern_Length(byte PPQN) {
  uint16_t patLgth = ((CVin[0].enable && CVin[0].dest_Selected == 3) || (CVin[1].enable && CVin[1].dest_Selected == 3)) ? modulated_pattern_Length : bank[currentBank].pattern[currentPattern].pattern_Length;
  patLgth = (PPQN == 4) ? patLgth : patLgth * 4;
  return patLgth;
}

uint16_t get_track_Length(byte track, byte PPQN) {
  uint16_t patLgth = ((CVin[0].enable && CVin[0].dest_Selected == 3) || (CVin[1].enable && CVin[1].dest_Selected == 3)) ? modulated_pattern_Length : bank[currentBank].pattern[currentPattern].track_Length[track];
  patLgth = (PPQN == 4) ? patLgth : patLgth * 4;
  return patLgth;
}

uint16_t get_firstStep(byte PPQN) {
  byte multip = (PPQN == 16) ? 4 : 1;
  uint16_t x = ((CVin[0].enable && CVin[0].dest_Selected == 4) || (CVin[1].enable && CVin[1].dest_Selected == 4)) ? modulated_firstStep : bank[currentBank].pattern[currentPattern].firstStep;
  return x * multip;
}


byte get_LFO_wave_Display(bool lfoNum) {
  byte x = ((CVin[0].enable && CVin[0].dest_Selected == 5) || (CVin[1].enable && CVin[1].dest_Selected == 5)) ? ((lfoNum == 0) ? modulated_lfo1Wave : modulated_lfo2Wave) : bank[currentBank].pattern[currentPattern].lfo_wave[lfoNum];
  return x;
}


byte get_LFO_rate_Display(bool lfoNum) {
  byte x = ((CVin[0].enable && CVin[0].dest_Selected == 6) || (CVin[1].enable && CVin[1].dest_Selected == 6)) ? ((lfoNum == 0) ? modulated_lfo1Rate : modulated_lfo2Rate) : bank[currentBank].pattern[currentPattern].lfo_rate[lfoNum];
  return x;
}


byte get_LFO2_rate_Display() {
  byte x = ((CVin[0].enable && CVin[0].dest_Selected == 8) || (CVin[1].enable && CVin[1].dest_Selected == 8)) ? modulated_lfo2Rate : bank[currentBank].pattern[currentPattern].lfo_rate[1];
  return x;
}


uint16_t get_track_step_TRIGS(byte track, bool step_advance) {
  int16_t curStep = ((CVin[0].enable && (CVin[0].dest_Selected == 2)) || (CVin[1].enable && (CVin[1].dest_Selected == 2))) ? modulated_global_Step : track_Step_16PPQN[track];
  curStep += step_advance;
  curStep += bank[currentBank].pattern[currentPattern].shiftValue[track] * 4;
  uint16_t loop_LengthDisplay = get_track_Length(track, 16);
  return (curStep < 0) ? (curStep + loop_LengthDisplay) : (curStep > loop_LengthDisplay - 1) ? (curStep - loop_LengthDisplay) : curStep;
}


byte get_track_step_PITCH(byte track, bool step_advance) {
  //bool tracksRetrig = bank[currentBank].pattern[currentPattern].tracksRetrig;
  //int16_t curStep = ((CVin[0].enable && (CVin[0].dest_Selected == 2)) || (CVin[1].enable && (CVin[1].dest_Selected == 2))) ? modulated_global_Step : (tracksRetrig) ? global_Step_4PPQN : track_Step_16PPQN[track] / 4;
  int16_t curStep = ((CVin[0].enable && (CVin[0].dest_Selected == 2)) || (CVin[1].enable && (CVin[1].dest_Selected == 2))) ? modulated_global_Step : track_Step_16PPQN[track] / 4;
  curStep += step_advance;
  curStep += bank[currentBank].pattern[currentPattern].shiftValue[track];
  byte loop_LengthDisplay = get_track_Length(track, 4);
  /*byte trackStepVal = (shiftedStep < 0) ? (shiftedStep + pattern_LengthDisplay) : (shiftedStep > pattern_LengthDisplay - 1) ? (shiftedStep - pattern_LengthDisplay) : shiftedStep;
    return trackStepVal;*/

  return (curStep < 0) ? (curStep + loop_LengthDisplay) : (curStep > loop_LengthDisplay - 1) ? (curStep - loop_LengthDisplay) : curStep;
}



unsigned int get_decimalValue_Seq() {
  byte currentBankDisplay = get_currentBank_Display(0);
  byte currentPatternDisplay = get_currentPattern_Display(0);

  for (byte track = 0; track < TRACKS; ++track) {
    bool can_Play = true;
    uint8_t byteToRead_solo = (track + 12) / 8;
    uint8_t bitPos_solo = (track + 12) & 7;
    uint8_t byteToRead_mute = track / 8; // used for muteState + fillState
    uint8_t bitPos_mute = track & 7;     // used for muteState + fillState
    if ( (screenConfig == 1 || screenConfig == 6 || screenConfig == 8) && (read_Seq_dont_play_this[track]) ) {
      can_Play = false;
      //if (clock_time_4PPQN == clock_time_16PPQN)
      if ((global_Step_16PPQN % 4 == 0) && (screenConfig != 8))
        read_Seq_dont_play_this[track] = false;
    }

    else {
      can_Play = (bank[currentBankDisplay].pattern[currentPatternDisplay].solo_active) ?
                 (((bank[currentBankDisplay].pattern[currentPatternDisplay].mute_solo_States[byteToRead_solo] & masks[bitPos_solo]) > 0) ? true : false) :
                 (((bank[currentBankDisplay].pattern[currentPatternDisplay].mute_solo_States[byteToRead_mute] & masks[bitPos_mute]) > 0) ? false : true);
    }

    if (can_Play) {
      uint16_t currentStep = get_track_step_TRIGS(track, 0);
      uint8_t byteToRead = currentStep / 8;
      uint8_t bitPos = currentStep & 7;

      byte realTrack = track + bank[currentBankDisplay].pattern[currentPatternDisplay].AB_State[track];

      bool fillData = ((bank[currentBankDisplay].pattern[currentPatternDisplay].fillState[byteToRead_mute] & masks[bitPos_mute]) > 0) ?
                      (((global_Step_16PPQN % bank[currentBankDisplay].pattern[currentPatternDisplay].fill_rate[track]) == 0) ? 1 : 0) : 0;

      if ((track == selected_Track) && (screenConfig == 5) && recMode)  // on buffer la sequence de fills
        fillDataRecording = fillData;

      trigsOut_BitVal[track] = ((bank[currentBankDisplay].pattern[currentPatternDisplay].Trigs[realTrack][byteToRead] & masks[bitPos]) > 0) ||
                               fillData;

      if (trigsOut_BitVal[track]) {
        read_Seq_dont_play_this[track] = false;
      }
    }
    else
      trigsOut_BitVal[track] = 0;
  }

  if ((global_Step_16PPQN % bank[currentBank].pattern[currentPattern].clkOut_Div) == 0)
    trigsOut_BitVal[12] = 1;
  else
    trigsOut_BitVal[12] = 0;

  trigsOut_BitVal[13] = btnsIn_BitVal[1]; //remoteBtn_Back_State;
  trigsOut_BitVal[14] = 0;
  trigsOut_BitVal[15] = 0;

  unsigned int result = 0;
  for (byte j = 0; j < DATA_WIDTH; ++j) {
    result += trigsOut_BitVal[j] << ((DATA_WIDTH - 1) - j);
  }

  return result;
}


unsigned int get_decimalValue_Seq2() {
  byte currentBankDisplay = get_currentBank_Display(0);
  byte currentPatternDisplay = get_currentPattern_Display(0);
  bool trigsOut_BitVal2[DATA_WIDTH];

  for (byte track = 0; track < TRACKS; ++track) {
    trigsOut_BitVal2[DATA_WIDTH] = 0;
    bool can_Play = true;
    uint8_t byteToRead_solo = (track + 12) / 8;
    uint8_t bitPos_solo = (track + 12) & 7;
    uint8_t byteToRead_mute = track / 8; // used for muteState + fillState
    uint8_t bitPos_mute = track & 7;     // used for muteState + fillState
    if ( (screenConfig == 1 || screenConfig == 6 || screenConfig == 8) && (read_Seq_dont_play_this[track] /*|| trigsIn_BitVal[track]*/) ) {
      can_Play = false;
      //if (clock_time_4PPQN == clock_time_16PPQN)
      if (global_Step_16PPQN % 4 == 0)
        read_Seq_dont_play_this[track] = false;
    }

    else {
      can_Play = (bank[currentBankDisplay].pattern[currentPatternDisplay].solo_active) ?
                 (((bank[currentBankDisplay].pattern[currentPatternDisplay].mute_solo_States[byteToRead_solo] & masks[bitPos_solo]) > 0) ? true : false) :
                 (((bank[currentBankDisplay].pattern[currentPatternDisplay].mute_solo_States[byteToRead_mute] & masks[bitPos_mute]) > 0) ? false : true);
    }

    if (can_Play && (bank[currentBankDisplay].pattern[currentPatternDisplay].track_isGate[track] == 1)) {
      uint16_t prevStep = get_track_step_TRIGS(track, 0);
      uint8_t prev_byteToRead = prevStep / 8;
      uint8_t prev_bitPos = prevStep & 7;

      uint16_t nextStep = get_track_step_TRIGS(track, 1);
      uint8_t byteToRead = nextStep / 8;
      uint8_t bitPos = nextStep & 7;

      byte realTrack = track + bank[currentBankDisplay].pattern[currentPatternDisplay].AB_State[track];

      bool fillData = ((bank[currentBankDisplay].pattern[currentPatternDisplay].fillState[byteToRead_mute] & masks[bitPos_mute]) > 0) ?
                      (((global_Step_16PPQN % bank[currentBankDisplay].pattern[currentPatternDisplay].fill_rate[track]) == 0) ? 1 : 0) : 0;

      if ((track == selected_Track) && (screenConfig == 5) && recMode)  // on buffer la sequence de fills
        fillDataRecording = fillData;

      if ((bank[currentBankDisplay].pattern[currentPatternDisplay].Trigs[realTrack][prev_byteToRead] & masks[prev_bitPos]) > 0) {
        trigsOut_BitVal2[track] = ((bank[currentBankDisplay].pattern[currentPatternDisplay].Trigs[realTrack][byteToRead] & masks[bitPos]) > 0) ||
                                  fillData;
      }

      else
        trigsOut_BitVal2[track] = 0;

      if (trigsOut_BitVal2[track]) {
        read_Seq_dont_play_this[track] = false;
      }
    }
    else
      trigsOut_BitVal2[track] = 0;
  }

  if ((global_Step_16PPQN % bank[currentBank].pattern[currentPattern].clkOut_Div) == 0)
    trigsOut_BitVal2[12] = 1;
  else
    trigsOut_BitVal2[12] = 0;

  trigsOut_BitVal2[13] = btnsIn_BitVal[1]; //remoteBtn_Back_State;
  trigsOut_BitVal2[14] = 0;
  trigsOut_BitVal2[15] = 0;

  unsigned int result = 0;
  for (byte j = 0; j < DATA_WIDTH; ++j) {
    result += trigsOut_BitVal2[j] << ((DATA_WIDTH - 1) - j);
  }

  return result;
}



void startUpSequence(void) {
  drawPage_startupScreen();
  for (byte i = 0; i < 4; ++i) {
    strip.setPixelColor(i, playBtn);
    strip.setPixelColor(i + 4, playBtn);
    strip.setPixelColor(i + 8, playBtn);
    strip.show();
    delay(80);
    strip.setPixelColor(i, 0);
    strip.setPixelColor(i + 4, 0);
    strip.setPixelColor(i + 8, 0);
    strip.show();
  }

  digitalWrite(altBtnLed , HIGH);
  delay(50);
  digitalWrite(altBtnLed , LOW);

  delay(200);
  u8g2.clear();
}


void reset_display_Screen() {
  (rollMode) ? screenConfig = 8 : screenConfig = 1;
  pitchMode = 0;
  MICROTIMING = 0;
  stepSeq_clickedStep = 255;
  selectedSeqStep_byte = 0;
  selectedSeqStep_bitPos = 0;
  //ALT_pushed = false;
  Trigs_Undo_enable = 0;
  Pitch_Undo_enable = 0;
  digitalWrite(altBtnLed, LOW);
  update_screen_INIT = true;
}


void modulate_currentPattern(byte cvInNum) {
  int cv_Input_Value = analogRead(cvInNum);
  int cv_Input_Scaled = map(cv_Input_Value, 0, 1023, /*PATTERNS - 1*/ 0, /*-(PATTERNS - 1)*/ 11);

  modulated_currentPattern = cv_Input_Scaled;

  //if (modulated_currentPattern > PATTERNS - 1)
  //modulated_currentPattern = PATTERNS - 1;

  //return cv_Input_Scaled;
}


byte modulate_currentBank(byte cvInNum) {
  int cv_Input_Value = analogRead(cvInNum);
  int cv_Input_Scaled = map(cv_Input_Value, 0, 1023, BANKS - 1, -(BANKS - 1));

  modulated_currentBank = cv_Input_Scaled;

  if (modulated_currentBank > 11) modulated_currentBank = 11;

  return modulated_currentBank;
}


uint16_t modulate_pattern_Length(byte cvInNum, uint16_t lastPatLength) {
  int cv_Input_Value = analogRead(cvInNum);
  uint16_t cv_Input_Scaled = map(cv_Input_Value, 0, 1023, 1, lastPatLength);

  modulated_pattern_Length = cv_Input_Scaled;

  if (modulated_pattern_Length < 1) modulated_pattern_Length = 1;
  else if (modulated_pattern_Length > MAX_STEPS_16PPQN) modulated_pattern_Length = MAX_STEPS_16PPQN;

  return modulated_pattern_Length;
}


byte modulate_firstStep(byte cvInNum) {
  int cv_Input_Value = analogRead(cvInNum);
  //uint16_t pattern_LengthDisplay = get_pattern_Length();
  byte lastStep = get_pattern_Length(4) - 1;
  byte cv_Input_Scaled = map(cv_Input_Value, 0, 1023, lastStep, bank[currentBank].pattern[currentPattern].firstStep);

  modulated_firstStep = cv_Input_Scaled;

  //if (modulated_firstStep > lastStep) modulated_firstStep = lastStep;

  return modulated_firstStep;
}


byte modulate_current_Step(byte cvInNum) {
  int cv_Input_Value = analogRead(cvInNum);
  byte lastStep = get_pattern_Length(4) - 1;
  byte cv_Input_Scaled = map(cv_Input_Value, 0, 1023, lastStep, bank[currentBank].pattern[currentPattern].firstStep);

  modulated_global_Step = cv_Input_Scaled;

  //if (modulated_global_Step > lastStep) modulated_global_Step = lastStep;

  return modulated_global_Step;
}



byte modulate_lfo1_Wave(byte cvInNum) {
  byte arrSize = sizeof(LFOs_waves) / sizeof(LFOs_waves[0]);
  int cv_Input_Value = analogRead(cvInNum);
  byte cv_Input_Scaled = map(cv_Input_Value, 0, 1023, arrSize - 1, 0);

  modulated_lfo1Wave = cv_Input_Scaled;

  //if (modulated_lfo1Wave > arrSize-1) modulated_lfo1Wave = arrSize-1;

  return modulated_lfo1Wave;
}

byte modulate_lfo2_Wave(byte cvInNum) {
  byte arrSize = sizeof(LFOs_waves) / sizeof(LFOs_waves[0]);
  int cv_Input_Value = analogRead(cvInNum);
  byte cv_Input_Scaled = map(cv_Input_Value, 0, 1023, arrSize - 1, 0);

  modulated_lfo2Wave = cv_Input_Scaled;

  //if (modulated_lfo2Wave > arrSize-1) modulated_lfo2Wave = arrSize-1;

  return modulated_lfo2Wave;
}

byte modulate_lfo1_Rate(byte cvInNum) {
  byte arrSize = sizeof(LFOs_rates) / sizeof(LFOs_rates[0]);
  int cv_Input_Value = analogRead(cvInNum);
  byte cv_Input_Scaled = map(cv_Input_Value, 0, 1023, arrSize - 1, 0);

  modulated_lfo1Rate = cv_Input_Scaled;

  //if (modulated_lfo1Rate > arrSize-1) modulated_lfo1Rate = arrSize-1;

  update_LFO_Rate_micros();

  return modulated_lfo1Rate;
}

byte modulate_lfo2_Rate(byte cvInNum) {
  byte arrSize = sizeof(LFOs_rates) / sizeof(LFOs_rates[0]);
  int cv_Input_Value = analogRead(cvInNum);
  byte cv_Input_Scaled = map(cv_Input_Value, 0, 1023, arrSize - 1, 0);

  modulated_lfo2Rate = cv_Input_Scaled;

  //if (modulated_lfo2Rate > arrSize-1) modulated_lfo2Rate = arrSize-1;

  update_LFO_Rate_micros();

  return modulated_lfo2Rate;
}

/*
  byte modulate_rollRate(bool cvInNum) {
  int cv_Input_Value = analogRead(cvInNum);
  byte arrSize = sizeof(rollRates) / sizeof(rollRates[0]);
  byte cv_Input_Scaled = map(cv_Input_Value, 0, 1023, 0, arrSize); // rollRates[10] = {1, 2, 3, 4, 6, 8, 12, 16, 32, 64};  rollRates_cursor

  modulated_rollRate = cv_Input_Scaled;

  if (modulated_rollRate > arrSize) modulated_rollRate = arrSize;

  return modulated_rollRate;
  }
*/

void modulateParams(byte paramSel_1, byte paramSel_2) {
  if (CVin[0].enable) {
    switch (paramSel_1) {
      case 0:
        modulate_currentPattern(CV_IN_1);
        break;

      case 1:
        //modulate_currentBank(CV_IN_1); A REMETTRE QUAND STOCKAGE SUFFISANT POUR BANKS
        break;

      case 2:
        modulate_current_Step(CV_IN_1);

        break;

      case 3:
        modulate_pattern_Length(CV_IN_1, get_pattern_Length(16));

        break;

      case 4:
        modulate_firstStep(CV_IN_1);
        break;

      case 5:
        modulate_lfo1_Wave(CV_IN_1);
        break;

      case 6:
        modulate_lfo1_Rate(CV_IN_1);
        break;

      case 7:
        modulate_lfo2_Wave(CV_IN_1);
        break;

      case 8:
        modulate_lfo2_Rate(CV_IN_1);
        break;
    }
  }

  if (CVin[1].enable) {
    switch (paramSel_2) {
      case 0:
        modulate_currentPattern(CV_IN_2);
        break;

      case 1:
        //modulate_currentBank(CV_IN_1); A REMETTRE QUAND STOCKAGE SUFFISANT POUR BANKS
        break;

      case 2:
        modulate_current_Step(CV_IN_2);

        break;

      case 3:
        modulate_pattern_Length(CV_IN_2, get_pattern_Length(16));

        break;

      case 4:
        modulate_firstStep(CV_IN_2);
        break;

      case 5:
        modulate_lfo1_Wave(CV_IN_2);
        break;

      case 6:
        modulate_lfo1_Rate(CV_IN_2);
        break;

      case 7:
        modulate_lfo2_Wave(CV_IN_2);
        break;

      case 8:
        modulate_lfo2_Rate(CV_IN_2);
        break;
    }
  }
}


bool checkIf_trigs(byte old_patLength, byte new_patLength) { // checkIf_trigs(last_pattern_Length, bank[currentBank].pattern[currentPattern].pattern_Length);
  bool goTo_test2 = false;
  /*TEST 1 : on regarde si il y a des trigs dans les nouvelles bars.
    Si oui (1) on return et on quitte replicate_bars
    Si non (0) on passe au TEST 2*/


  for (byte track = 0; track < TRACKS_AB; ++track) {
    for (uint16_t steps = old_patLength * 4; steps < new_patLength * 4; ++steps) {
      uint8_t byteToRead = steps / 8;
      uint8_t bitPos = steps & 7;
      if ((bank[currentBank].pattern[currentPattern].Trigs[track][byteToRead] & masks[bitPos]) > 0) {
        goTo_copy = false;
        goto done;
      }
      else goTo_test2 = true;
    }
  }

  /*TEST 2 : on regarde si il y a des trigs dans les anciennes bars.
    Si oui (1) on dit goTo_copy
    Si non on return et on quitte replicate_bars*/
  if (goTo_test2) {
    for (byte track = 0; track < TRACKS_AB; ++track) {
      for (uint16_t steps = 0; steps < old_patLength * 4; ++steps) {
        uint8_t byteToRead = steps / 8;
        uint8_t bitPos = steps & 7;

        if ((bank[currentBank].pattern[currentPattern].Trigs[track][byteToRead] & masks[bitPos]) > 0) {
          goTo_copy = true;
          goto done;
        }
        else
          goTo_copy = false;
      }
    }
  }

done :
  return goTo_copy;
}


void checkIf_patternEmpty(byte ze_bank, byte ze_pattern) {
  for (int i = 0; i < (TRACKS_AB); ++i) {
    uint8_t byteToRead = i / 8;
    uint8_t bitPos = i & 7;

    if ((bank[ze_bank].pattern[ze_pattern].track_has_Trigs[byteToRead] & masks[bitPos]) > 0) {
      bank[ze_bank].notEmpty_Pattern[ze_pattern] = 1;
      break;
    }
    else {
      bank[ze_bank].notEmpty_Pattern[ze_pattern] = 0;
    }
  }
}


void checkIf_glide_running(bool glideIsSetToActive) {
  for (byte dac = 0; dac < 4; ++dac) {
    byte cvTrack = bank[currentBank].pattern[currentPattern].cvOut_Tracks[dac];
    if ((cvTrack == 12) || (bank[currentBank].pattern[currentPattern].glide_steps[cvTrack] == 0)) {
      DaC[dac].glideActive = 0;
      if (!glideIsSetToActive) {
        bank[currentBank].pattern[currentPattern].glide_active = 0;
        GLIDE_timer.disableTimer();
      }
    }

    else {
      DaC[dac].glideActive = 1;
      if (!glideIsSetToActive) {
        bank[currentBank].pattern[currentPattern].glide_active = 1;
        glideIsSetToActive = true;
        GLIDE_timer.enableTimer();
      }
    }
  }
}


void reset_Pattern(byte ze_bank, byte ze_pattern) {
  for (byte track = 0; track < TRACKS_AB; ++track) {
    for (uint16_t steps = 0; steps < MAX_STEPS_16PPQN; ++steps) {
      clearSequence(ze_bank, ze_pattern, track, steps, 1, 1);
    }
    bank[ze_bank].pattern[ze_pattern].transposeValue[track] = 0;
  }

  for (byte track = 0; track < TRACKS; ++track) {
    bank[ze_bank].pattern[ze_pattern].shiftValue[track] = 0; //if (bank[currentBank].pattern[currentPattern].fill_active)
    bank[ze_bank].pattern[ze_pattern].glide_mode[track] = 0;
    bank[ze_bank].pattern[ze_pattern].glide_steps[track] = 0;
    bank[ze_bank].pattern[ze_pattern].fill_rate[track] = 16;
    bank[ze_bank].pattern[ze_pattern].AB_State[track] = 0;
    bank[ze_bank].pattern[ze_pattern].track_Length[track] = 32;

    bank[ze_bank].pattern[ze_pattern].track_isGate[track] = 0;

    uint8_t byteToWrite = track / 8; // used for muteState + fillState
    uint8_t bitPos = track & 7;      // used for muteState + fillState
    uint8_t byteToWrite_solo = (track + 12) / 8;
    uint8_t bitPos_solo = (track + 12) & 7;

    bank[ze_bank].pattern[ze_pattern].mute_solo_States[byteToWrite] &= ~masks[bitPos];
    bank[ze_bank].pattern[ze_pattern].mute_solo_States[byteToWrite_solo] &= ~masks[bitPos_solo];

    bank[ze_bank].pattern[ze_pattern].fillState[byteToWrite] &= ~masks[bitPos];

    bank[ze_bank].pattern[ze_pattern].track_has_Trigs[byteToWrite] &= ~masks[bitPos];
    bank[ze_bank].pattern[ze_pattern].track_has_Trigs[byteToWrite_solo] &= ~masks[bitPos_solo];
    if (track < 4)
      bank[ze_bank].pattern[ze_pattern].cvOut_Tracks[track] = 12;
  }

  //bank[ze_bank].pattern[ze_pattern].internal_BPM = 120.0;
  bank[ze_bank].pattern[ze_pattern].internal_BPM = 120;

  bank[ze_bank].pattern[ze_pattern].clkIn_Div = 1;
  bank[ze_bank].pattern[ze_pattern].clkOut_Div = 1;

  bank[ze_bank].pattern[ze_pattern].quantizeBeat = 1;
  bank[ze_bank].pattern[ze_pattern].shuffle = 0;

  bank[ze_bank].pattern[ze_pattern].pattern_Length = 32;
  bank[ze_bank].pattern[ze_pattern].mute_active = 0;
  bank[ze_bank].pattern[ze_pattern].allMute_active = 0;
  bank[ze_bank].pattern[ze_pattern].solo_active = 0;
  bank[ze_bank].pattern[ze_pattern].fill_active = 0;
  bank[ze_bank].pattern[ze_pattern].allAorB = 0;
  bank[ze_bank].pattern[ze_pattern].shift_active = 0;
  bank[ze_bank].pattern[ze_pattern].glide_active = 0;
  bank[ze_bank].pattern[ze_pattern].firstStep = 0;
  bank[ze_bank].pattern[ze_pattern].tracksRetrig = 1;


  for (byte lfo = 0; lfo < 2; ++lfo) {
    bank[ze_bank].pattern[ze_pattern].lfo_enable[lfo] = 0;
    bank[ze_bank].pattern[ze_pattern].lfo_dest[lfo] = 0;
    bank[ze_bank].pattern[ze_pattern].lfo_wave[lfo] = 0;
    bank[ze_bank].pattern[ze_pattern].lfo_rate[lfo] = 5;
    bank[ze_bank].pattern[ze_pattern].lfo_gain[lfo] = 100;
    bank[ze_bank].pattern[ze_pattern].lfo_phase[lfo] = 0;
  }

  bank[ze_bank].notEmpty_Pattern[ze_pattern] = 0;
  //pattern_Length_16PPQN = bank[ze_bank].pattern[ze_pattern].pattern_Length * 4;
  stepSeq_page = 0;
}


void resetBank(byte ze_bank) {
  for (byte pat = 0; pat < PATTERNS; ++pat) {
    reset_Pattern(ze_bank, pat);
  }

  for (byte songStep = 0; songStep < 16; ++songStep) {
    bank[ze_bank].songPatterns[songStep] = 255;
  }

  stepSeq_page = 0;
}


void loadBank(byte bankNum) {
  if (EEPROM.read(BANK_ADR_0) != 0xFF) {
    EEPROM.get(BANK_ADR_0, bank[bankNum].notEmpty_Pattern);

    for (byte i = 0; i < PATTERNS; ++i) {
      if (EEPROM.read(patterns_addresses[i]) != 255) {
        EEPROM.get(patterns_addresses[i], bank[bankNum].pattern[i]);
      }
    }
  }

  else {
    EEPROM.put(BANK_ADR_0, bank[currentBank].notEmpty_Pattern);
    for (byte i = 0; i < PATTERNS; ++i) {
      EEPROM.put(patterns_addresses[i], bank[bankNum].pattern[i]);
    }
  }
}


void LFOs_enable_disable() {
  if (bank[currentBank].pattern[currentPattern].lfo_enable[0] == 1)
    LFO1_timer.enableTimer();
  else
    LFO1_timer.disableTimer();


  if (bank[currentBank].pattern[currentPattern].lfo_enable[1] == 1)
    LFO2_timer.enableTimer();
  else
    LFO2_timer.disableTimer();
}


uint32_t get_LFO_Rate_micros(byte lfoNum) {
 unsigned int rate = step_micros * 16;

  switch (get_LFO_rate_Display(lfoNum)) {
    case 0:
      rate = step_micros * 128;
      break;

    case 1:
      rate = step_micros * 96;
      break;

    case 2:
      rate = step_micros * 64;
      break;

    case 3:
      rate = step_micros * 48;
      break;

    case 4:
      rate = step_micros * 32;
      break;

    case 5:
      rate = step_micros * 16;
      break;

    case 6:
      rate = step_micros * 8;
      break;

    case 7:
      rate = (step_micros * 16) / 3;
      break;

    case 8:
      rate = step_micros * 4;
      break;

    case 9:
      rate = (step_micros * 16) / 6;
      break;

    case 10:
      rate = step_micros * 2;
      break;

    case 11:
      rate = step_micros;
      break;

    case 12:
      rate = step_micros / 2;
      break;
  }

  return rate / LFO_WAVE_RESOLUTION;
}



void buffer_Sequences() {
  for (byte track = 0; track < TRACKS_AB; ++track) {
    for (uint16_t steps = 0; steps < MAX_STEPS_16PPQN / 8; ++steps) {
      Trigs_Undo_buffer[track][steps] = bank[currentBank].pattern[currentPattern].Trigs[track][steps];
    }
  }

  for (byte track = 0; track < TRACKS_AB; ++track) {
    for (byte steps = 0; steps < MAX_STEPS_4PPQN; ++steps) {
      Pitch_Undo_buffer[track][steps] = bank[currentBank].pattern[currentPattern].Pitch[track][steps];
    }
  }
}


void clearSequence(byte zebank, byte zepattern, byte track, uint16_t steps, bool delete_trigs, bool delete_pitch) {
  if (delete_trigs == 1) {
    Trigs_Undo_enable = 1;
    bank[zebank].pattern[zepattern].Trigs[track][steps / 8] &= ~masks[steps & 7];
  }

  if (delete_pitch == 1) {
    Pitch_Undo_enable = 1;
    bank[zebank].pattern[zepattern].Pitch[track][steps / 4] = 24;
  }

  bank[zebank].pattern[zepattern].transposeValue[track] = 0;
  fillDataRecording_startStep = 0;

  if (screenConfig == 6)
    update_screen_PADS = true;
}


void assign_CVout_track(byte padID) {
  byte cvTrack = bank[currentBank].pattern[currentPattern].cvOut_Tracks[padID];

  if (cvTrack == selected_Track) { // si le pad enfoncé est le currentTrack
    bank[currentBank].pattern[currentPattern].cvOut_Tracks[padID] = 12;
  }

  else {
    if (cvTrack == 12) {
      bank[currentBank].pattern[currentPattern].cvOut_Tracks[padID] = selected_Track;
    }

    else {
      trigger_cvOutReassign_process = true;
    }
  }
}


void reset_ChromaMode_UI() {
  chromaMode_cursor = 0;
  chromaMode_KBoard = 1;
  chromaMode_Arp = 0;
  chromaMode_Arp_params = 0;
  chromaMode_Transpose = 0;
  chromaMode_Glide = 0;
  chromaMode_CVOutSelect = 0;
  trigger_cvOutReassign_process = false;
  confirmed_CvOutReassign = 0;
}


bool LFO_enable_State(byte lfoNum) {
  bool enable_State;
  byte lfoTrack = (lfoNum == 0) ? 13 : 14;
  for (byte cvOut = 0; cvOut < 4; ++cvOut) {
    if (bank[currentBank].pattern[currentPattern].cvOut_Tracks[cvOut] == lfoTrack) {
      enable_State = 1;
      break;
    }
    else
      enable_State = 0;
  }

  return enable_State;
}


void loadPattern_params() { // re-synchronise le nouveau pattern selected
  if (!BPM_isGlobal && SYNC_mode_internal) {
    step_micros = calc_step_micros();
    CLOCK_INT_timer.setInterval(step_micros, do_MASTERClock);
  }

  calc_GLIDE_time();
  get_RollRateCalc();

  update_LFO_Rate_micros();
  LFOs_enable_disable();
}


void scan_Pads_Arp() {
  arpSteps = 0;
  scan_pads_Arp = 11;

  if (arp_Style_Cursor == 0) {//UP
    for (byte pad = 0; pad < TRACKS; ++pad) {
      if (trigsIn_BitVal[pad]) {

        arpNotes[arpSteps] = pad + octaveValue;

        if (arpSteps < ARP_MAX_STEPS - 1)
          arpSteps++;

        if (arp_latch)
          arp_latch_pads[pad] = 1;
      }
    }
  }

  else if (arp_Style_Cursor == 1) {  //DOWN
    for (byte pad = 11; pad > 0; --pad) {
      if (trigsIn_BitVal[pad]) {

        arpNotes[arpSteps] = pad + octaveValue;

        if (arpSteps < ARP_MAX_STEPS - 1)
          arpSteps++;

        if (arp_latch)
          arp_latch_pads[pad] = 1;
      }
    }
  }
}


void scan_Pads_Arp2() {
  arpSteps = 0;
  scan_pads_Arp = 22;

  if (arp_Style_Cursor == 0) {//UP
    for (byte pad = 0; pad < TRACKS; ++pad) {
      if (trigsIn_BitVal[pad]) {

        arpNotes[arpSteps] = pad + octaveValue;

        if (arpSteps < ARP_MAX_STEPS - 1)
          arpSteps++;

        if (arp_latch)
          arp_latch_pads[pad] = 1;
      }
    }
  }

  else if (arp_Style_Cursor == 1) {  //DOWN
    for (byte pad = 11; pad > 0; --pad) {
      if (trigsIn_BitVal[pad]) {

        arpNotes[arpSteps] = pad + octaveValue;

        if (arpSteps < ARP_MAX_STEPS - 1)
          arpSteps++;

        if (arp_latch)
          arp_latch_pads[pad] = 1;
      }
    }
  }
}


void recordFills() {
  if (fillDataRecording) {
    uint16_t stepToWrite = get_track_step_TRIGS(selected_Track, 0);
    uint8_t bitPos_trigs = stepToWrite & 7;
    byte realTrack = selected_Track + bank[currentBank].pattern[currentPattern].AB_State[selected_Track];
    uint8_t byteToWrite_trigs = stepToWrite / 8;

    uint8_t byteToWrite_notEmpty = realTrack / 8;
    uint8_t bitPos_notEmpty = realTrack & 7;

    bank[currentBank].pattern[currentPattern].Trigs[realTrack][byteToWrite_trigs] |= masks[bitPos_trigs];
    bank[currentBank].pattern[currentPattern].track_has_Trigs[byteToWrite_notEmpty] |= masks[bitPos_notEmpty];
    bank[currentBank].notEmpty_Pattern[currentPattern] = 1;
  }
}


/*

  CVINS voltages

  -10V : 1023
  -5V  : 770
  -2.5V : 513
  0V : 255
  +2.5V : 0
  +5V  : 0
  +10V : 0




  SETTINGS

  case 1: // MAIN SCREEN
  case 2: // PATTERN SELECTION
  case 3: // A/B
  case 4: // MUTE
  case 5: // FILL
  case 6: // DELETE STEPS
  case 7: // SETTINGS SCREEN
  case 8: // ROLL MODE
  case 11: // STEP SEQUENCER
  case 12: // PITCH SEQUENCER
  case 13: // PATTERN EDIT MODE
  case 14: // SONG MODE
  case 15: // BANK EDIT MODE
  case 16: // PITCH GLIDE MODE

  case 17: // SETTINGS - PATTERN
  case 18: // SETTINGS - LFO 1
  case 19: // SETTINGS - LFO 2
  case 20: // SETTINGS - GENERAL


  case 22: // BANK SELECTION
  case 33: // CHROMA MODE
  case 44: // SOLO
  case 55: // SHIFT
  case 66: // CLEAR TRACK


  si case 7 // settings
  afficher :


  - PATTERN --> vers
  -- "FIRSTSTEP"
  -- "CVin1"
  -- "CVin2"


  - LFO 1
  -- "LFO1 OUTPUT"
  -- "LFO1 WAVEFORM"
  -- "LFO1 RATE"
  -- "LFO1 GAIN"

  - LFO 2
  -- "LFO2 OUTPUT"
  -- "LFO2 WAVEFORM"
  -- "LFO2 RATE"
  -- "LFO2 GAIN"


  - GENERAL
  -- "SYNC"
  -- "BPM"
  -- "COPY BARS"
  -- "LED BRIGHT."


  const char* const settings_titles[] = {
  "FIRSTSTEP",

  "CVin1",
  "CVin2",

  "COPY BARS",
  "SYNC",
  "BPM",

  "LFO1 OUTPUT",
  "LFO1 WAVEFORM",
  "LFO1 RATE",
  "LFO1 GAIN",
  "LFO2 OUTPUT",
  "LFO2 WAVEFORM",
  "LFO2 RATE",
  "LFO2 GAIN",

  "LED BRIGHT."
  };


*/
