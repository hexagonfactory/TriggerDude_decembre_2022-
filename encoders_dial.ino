void poll_encoders() {
  bool encoder1_A_State = digitalRead(encoder1_A_Pin);
  enc1_dir = 0;
  if (encoder1_A_State != last_encoder1_A_State && millis() - last_enc_DebounceTime_millis[0] > DEBOUNCE_ENC_MILLIS) {
    last_enc_DebounceTime_millis[0] = millis();
    last_encoder1_A_State = encoder1_A_State;
    fired_1 = true;
    update_screen_POTS_DIAL_L = true;

    if (encoder1_A_State == 0) {
      if (digitalRead(encoder1_B_Pin) == 1)
        enc1_dir = 1;
      else
        enc1_dir = -1;

      //update_screen_POTS_DIAL_L = true;
    }

    /*else
      enc1_dir = 0;*/
  }

  bool encoder2_A_State = digitalRead(encoder2_A_Pin);
  enc2_dir = 0;
  if (encoder2_A_State != last_encoder2_A_State && millis() - last_enc_DebounceTime_millis[1] > DEBOUNCE_ENC_MILLIS) {
    last_enc_DebounceTime_millis[1] = millis();
    last_encoder2_A_State = encoder2_A_State;
    fired_2 = true;
    update_screen_POTS_DIAL_R = true;

    if (encoder2_A_State == 0) {
      if (digitalRead(encoder2_B_Pin) == 1)
        enc2_dir = 1;
      else
        enc2_dir = -1;

      //update_screen_POTS_DIAL_R = true;
    }

    /*else
      enc2_dir = 0;*/
  }


  //last_encoder1_Btn_State = 1;
  //last_encoder2_Btn_State = 1;

  bool encoder1_Btn_State = digitalRead(encoder1_Btn);  // default = 1
  bool encoder2_Btn_State = digitalRead(encoder2_Btn);  // default = 1

  if (encoder1_Btn_State != last_encoder1_Btn_State) {
    if (encoder1_Btn_State == 0) {
      enc1_clk = true;
      update_screen_POTS_CLK_L = true;
    }
    last_encoder1_Btn_State = encoder1_Btn_State;
  }

  /*if (digitalRead(encoder1_Btn) == 0 && last_encoder1_Btn_State == 1) {
      enc1_clk = true;
      update_screen_POTS_CLK_L = true;
    }*/

  if (encoder2_Btn_State != last_encoder2_Btn_State) {
    if (encoder2_Btn_State == 0) {
      enc2_clk = true;
      update_screen_POTS_CLK_R = true;
    }
    last_encoder2_Btn_State = encoder2_Btn_State;
  }

  //last_encoder1_Btn_State = 1;
  //last_encoder2_Btn_State = 1;
}


void set_track_FillRate() {
  byte fillRate_pos = 0;

  for (uint8_t i = 0; i < sizeof(fillRates) / sizeof(fillRates[0]); ++i) { //fill_rate
    if (bank[currentBank].pattern[currentPattern].fill_rate[selected_Track] == fillRates[i]) {
      fillRate_pos = i;
      break;
    }
  }

  if (enc1_dir == 1) {
    if (fillRate_pos < 6)
      fillRate_pos++;
  }

  else if (enc1_dir == -1) {
    if (fillRate_pos > 0)
      fillRate_pos--;
  }

  bank[currentBank].pattern[currentPattern].fill_rate[selected_Track] = fillRates[fillRate_pos];

  //update_screen = true;
  fired_1 = false;
  //return fillRateValue;
}


void set_allAorB() {
  if ((enc1_dir == 1) && !bank[currentBank].pattern[currentPattern].allAorB)
    bank[currentBank].pattern[currentPattern].allAorB = 1;

  else if ((enc1_dir == -1) && bank[currentBank].pattern[currentPattern].allAorB)
    bank[currentBank].pattern[currentPattern].allAorB = 0;

  //update_screen = true;
  fired_1 = false;
}


void set_selected_Track() {
  if ((enc1_dir == 1) && (selected_Track < TRACKS - 1))
    selected_Track++;

  else if ((enc1_dir == -1) && (selected_Track > 0))
    selected_Track--;

  reset_ChromaMode_UI();
  update_padLeds = true;
  //update_screen = true;

  fired_1 = false;
}


void set_stepSeq_page() {
  //byte pLength = (bank[currentBank].pattern[currentPattern].tracksRetrig) ? bank[currentBank].pattern[currentPattern].pattern_Length : get_track_Length(selected_Track, 4);
  byte pLength = get_track_Length(selected_Track, 4);
  if (enc2_dir == 1) {
    if (stepSeq_page < (pLength - 8)) {
      stepSeq_page += 8;
    }
  }

  else if (enc2_dir == -1) {
    if (stepSeq_page > 0) {
      stepSeq_page -= 8;
    }
  }

  //MICROTIMING = 0;
  stepSeq_clickedStep = 255;
  selectedSeqStep_byte = 0;
  selectedSeqStep_bitPos = 0;
  update_padLeds = true;
  fired_2 = false;
}


void set_Step_pitch(byte curTrack, byte curstep4PPQN) {
  //static unsigned long button_micro = 0;
  byte realTrack = curTrack + step_seq_ab[curTrack];
  //pitchPointer_Pot = pitchPointer_Pad;
  //pitchPointer_Pot = bank[currentBank].pattern[currentPattern].Pitch[realTrack][curstep4PPQN];

  byte arrSize = sizeof(notesValues) / sizeof(notesValues[0]);

  byte pot_target_h = (pitch_Range_12) ? (pitchPointer_Pot + 12) : (pitchPointer_Pot);
  byte pot_target_l = (pitch_Range_12) ? (pitchPointer_Pot - 12) : (pitchPointer_Pot);

  if ((enc1_dir == 1) && (pot_target_h < arrSize - 1)) {
    (pitch_Range_12) ? pitchPointer_Pot += 12 : pitchPointer_Pot++;
  }

  else if ((enc1_dir == -1) && (pot_target_l > 0)) {
    (pitch_Range_12) ? pitchPointer_Pot -= 12 : pitchPointer_Pot--;
  }

  bank[currentBank].pattern[currentPattern].Pitch[realTrack][curstep4PPQN] = pitchPointer_Pot;
  //pitchPointer_Pad = pitchPointer_Pot;

  uint16_t step16 = curstep4PPQN * 4;
  for (uint16_t x = step16 + 1; x < get_pattern_Length(16) - 1; ++x) {
    if (!((bank[currentBank].pattern[currentPattern].Trigs[realTrack][x / 8] & masks[x & 7]) > 0))
      bank[currentBank].pattern[currentPattern].Pitch[realTrack][x / 4] = pitchPointer_Pot;
    else
      break;
  }

  //button_micro = micros();
  button_micros[curTrack] = micros();

  //make595Play_Trigs_from_ChromaMode(decimalValue_from_chromaMode(), button_micros[curTrack]);
  //makeDACplayNotes_from_ChromaMode(pitchPointer_Pot, curTrack);
  fired_1 = false;
}


void set_ChromaMode_octave() {
  if ((enc1_dir == 1) && (octaveValue < 84)) {
    octaveValue += 12;
  }
  else if ((enc1_dir == -1) && (octaveValue > 0)) {
    octaveValue -= 12;
  }

  //update_screen = true;
  fired_1 = false;
}


void set_ChromaMode_octave_Arp() {
  if ((enc1_dir == 1) && (octaveValue < 84)) {
    octaveValue += 12;
  }
  else if ((enc1_dir == -1) && (octaveValue > 0)) {
    octaveValue -= 12;
  }

  //update_screen = true;
  fired_1 = false;
}


void set_ChromaMode_transpose() {
  byte realTrack = selected_Track + bank[currentBank].pattern[currentPattern].AB_State[selected_Track];
  int8_t tVal = bank[currentBank].pattern[currentPattern].transposeValue[realTrack];
  if ((enc1_dir == 1) && (tVal < 5)) {
    tVal++;
  }
  else if ((enc1_dir == -1) && (tVal > -2)) {
    tVal--;
  }

  bank[currentBank].pattern[currentPattern].transposeValue[realTrack] = tVal;

  update_padLeds = true;
  //update_screen = true;
  fired_1 = false;
}


void set_ChromaMode_glide_param_selection() {
  if ((enc1_dir == 1) && (GLIDE_paramSelection_cursor < 1)) {
    GLIDE_paramSelection_cursor = 1;
  }

  else if ((enc1_dir == -1) && (GLIDE_paramSelection_cursor > 0)) {
    GLIDE_paramSelection_cursor = 0;
  }

  //update_screen = true;
  fired_1 = false;
}


void set_glide_MODE() {
  byte currentPatternDisplay = get_currentPattern_Display(0);
  byte currentBankDisplay = get_currentBank_Display(0);
  bool gMode = bank[currentBank].pattern[currentPattern].glide_mode[selected_Track];

  if ((enc1_dir == 1) && (gMode == 0))
    gMode = 1;
  else if ((enc1_dir == -1) && (gMode == 1))
    gMode = 0;

  bank[currentPatternDisplay].pattern[currentBankDisplay].glide_mode[selected_Track] = gMode;

  //update_screen = true;
  fired_1 = false;
}


void calc_GLIDE_time() {
  byte currentPatternDisplay = get_currentPattern_Display(0);
  byte currentBankDisplay = get_currentBank_Display(0);

  for (byte dac = 0; dac < 4; ++dac) {
    if (DaC[dac].glideActive) {
      byte track = bank[currentBankDisplay].pattern[currentPatternDisplay].cvOut_Tracks[dac]; // 0 (tracks) à 12 (12 = pas de glide sur ce track)
      if (track < TRACKS) {
        DaC[dac].glide_duration_micros = (bank[currentBankDisplay].pattern[currentPatternDisplay].glide_steps[track] * step_micros) / 1000;
      }
    }
  }
}


void set_glide_STEPS() {
  byte currentPatternDisplay = get_currentPattern_Display(0);
  byte currentBankDisplay = get_currentBank_Display(0);
  byte gSteps = bank[currentBankDisplay].pattern[currentPatternDisplay].glide_steps[selected_Track];

  if ((enc1_dir == 1) && (gSteps < 4))
    gSteps++;
  else if ((enc1_dir == -1) && (gSteps > 0))
    gSteps--;

  bank[currentBankDisplay].pattern[currentPatternDisplay].glide_steps[selected_Track] = gSteps;

  for (byte dac = 0; dac < 4; ++dac) {
    DaC[dac].gliding = false;
    if (bank[currentBankDisplay].pattern[currentPatternDisplay].cvOut_Tracks[dac] == selected_Track) {
      if (gSteps == 0) {
        DaC[dac].glide_duration_micros = 0;
        DaC[dac].glideActive = 0;
        checkIf_glide_running(false);
      }
      else {
        DaC[dac].glide_duration_micros = (gSteps * step_micros) / 1000;
        bank[currentBank].pattern[currentPattern].glide_active = 1;
        DaC[dac].glideActive = 1;
        GLIDE_timer.enableTimer();
      }

      break;
    }
  }

  //update_screen = true;
  fired_1 = false;
}


void set_ChromaMode_UI() {
  byte arrSize = sizeof(chromaMode_UIs) / sizeof(chromaMode_UIs[0]);
  if ((enc2_dir == 1) && (chromaMode_cursor < arrSize - 1)) {
    chromaMode_cursor++;
  }

  else if ((enc2_dir == -1) && (chromaMode_cursor > 0)) {
    chromaMode_cursor--;
  }


  switch (chromaMode_cursor) {
    case 0:
      screenConfig = 33;
      reset_ChromaMode_UI();
      break;

    case 1:
      screenConfig = 34;
      chromaMode_KBoard = 0;
      chromaMode_Arp = 1;
      chromaMode_Transpose = 0;
      chromaMode_Glide = 0;
      chromaMode_CVOutSelect = 0;
      break;

    case 2:
      screenConfig = 35;
      chromaMode_KBoard = 0;
      chromaMode_Arp = 0;
      chromaMode_Transpose = 1;
      chromaMode_Glide = 0;
      chromaMode_CVOutSelect = 0;
      break;

    case 3:
      screenConfig = 36;
      chromaMode_KBoard = 0;
      chromaMode_Arp = 0;
      chromaMode_Transpose = 0;
      chromaMode_Glide = 1;
      chromaMode_CVOutSelect = 0;
      break;

    case 4:
      screenConfig = 37;
      chromaMode_KBoard = 0;
      chromaMode_Arp = 0;
      chromaMode_Transpose = 0;
      chromaMode_Glide = 0;
      chromaMode_CVOutSelect = 1;
      break;
  }

  update_padLeds = true;
  update_screen_INIT = true;
  fired_2 = false;
}


void set_rollRate() {
  byte arrSize = sizeof(rollRates) / sizeof(rollRates[0]);

  if ((enc1_dir == 1) && (rollRates_cursor < (arrSize - 1))) {
    rollRates_cursor++;
  }
  else if ((enc1_dir == -1) && (rollRates_cursor > 0)) {
    rollRates_cursor--;
  }

  get_RollRateCalc();

  //update_screen = true;
  //update_screen_POTS_DIAL_L = true;
  fired_1 = false;
}


void replicate_bars(/*uint16_t*/byte last_pat_length) { // last_pat_length = 16
  //byte bars = last_pat_length / 8;
  //byte bars = (last_pat_length * 4) / 8;
  byte bars = last_pat_length / 2;
  for (byte b = 0; b < bars; ++b) {
    for (byte track = 0; track < TRACKS_AB; ++track) {
      uint8_t byteToRead = track / 8;
      uint8_t bitPos = track & 7;
      //if (bank[currentBank].pattern[currentPattern].track_has_Trigs[track])
      if ((bank[currentBank].pattern[currentPattern].track_has_Trigs[byteToRead] & masks[bitPos]) > 0)
        bank[currentBank].pattern[currentPattern].Trigs[track] [b + bars] = bank[currentBank].pattern[currentPattern].Trigs[track][b];
    }
  }

  for (uint16_t b = 0; b < last_pat_length; ++b) {
    for (byte track = 0; track < TRACKS_AB; ++track) {
      uint8_t byteToRead = track / 8;
      uint8_t bitPos = track & 7;
      //if (bank[currentBank].pattern[currentPattern].track_has_Trigs[track])
      if ((bank[currentBank].pattern[currentPattern].track_has_Trigs[byteToRead] & masks[bitPos]) > 0)
        bank[currentBank].pattern[currentPattern].Pitch[track] [b + last_pat_length] = bank[currentBank].pattern[currentPattern].Pitch[track] [b];
    }
  }
}


void set_pattern_Length() {
  byte last_pattern_Length = bank[currentBank].pattern[currentPattern].pattern_Length; // on garde l'actuel pattern_Length avant changement

  if (!pattern_Length_Locked) {
    if (enc2_dir == 1) {
      if (bank[currentBank].pattern[currentPattern].pattern_Length < MAX_STEPS_4PPQN) {
        bank[currentBank].pattern[currentPattern].pattern_Length++;
      }
    }

    else if (enc2_dir == -1) {
      byte limit = (bank[currentBank].pattern[currentPattern].firstStep == 0) ? 1 : (bank[currentBank].pattern[currentPattern].firstStep + 1);
      if (bank[currentBank].pattern[currentPattern].pattern_Length > limit) {
        bank[currentBank].pattern[currentPattern].pattern_Length--;
      }
    }

    //if (bank[currentBank].pattern[currentPattern].tracksRetrig) {
    /*for (byte track = 0; track < TRACKS; ++track) {
      if (bank[currentBank].pattern[currentPattern].track_Length[track] == last_pattern_Length)
        bank[currentBank].pattern[currentPattern].track_Length[track] = bank[currentBank].pattern[currentPattern].pattern_Length;
      }*/
    //}
  }

  else {
    if (enc2_dir == 1) {
      unsigned int new_pattern_Length = last_pattern_Length * 2;
      if (new_pattern_Length <= MAX_STEPS_4PPQN) {
        bank[currentBank].pattern[currentPattern].pattern_Length = new_pattern_Length;

        checkIf_trigs(last_pattern_Length, new_pattern_Length);

        if (replicateBars && goTo_copy)
          replicate_bars(last_pattern_Length);
      }
    }

    else if (enc2_dir == -1) {
      byte new_pattern_Length = last_pattern_Length / 2;
      byte limit = (bank[currentBank].pattern[currentPattern].firstStep == 0) ? 1 : (bank[currentBank].pattern[currentPattern].firstStep + 1);

      if (new_pattern_Length > limit) {
        bank[currentBank].pattern[currentPattern].pattern_Length = new_pattern_Length;

        if (global_Step_4PPQN > new_pattern_Length) {
          global_Step_4PPQN = global_Step_4PPQN - new_pattern_Length;
          global_Step_16PPQN = global_Step_16PPQN - (new_pattern_Length * 4);
        }
      }
    }
  }

  //pattern_Length_16PPQN = bank[currentBank].pattern[currentPattern].pattern_Length * 4;

  stepSeq_page = 0;
  fired_2 = false;
}


void set_track_Length() {
  byte tLength = bank[currentBank].pattern[currentPattern].track_Length[selected_Track];

  if (enc1_dir == 1) {
    if (!enc1_Range_10) {
      if (tLength < MAX_STEPS_4PPQN) {
        tLength++;
      }
    }

    else {
      if (tLength <= MAX_STEPS_4PPQN - 10) {
        tLength += 10;
      }

      else if (tLength < MAX_STEPS_4PPQN) {
        tLength += (MAX_STEPS_4PPQN - tLength);
      }
    }
  }

  else if (enc1_dir == -1) {
    byte limit = (bank[currentBank].pattern[currentPattern].firstStep == 0) ? 1 : (bank[currentBank].pattern[currentPattern].firstStep + 1);

    if (!enc1_Range_10) {
      if (tLength > limit)
        tLength--;
    }

    else {
      if (tLength >= limit + 10)
        tLength -= 10;

      else if (tLength > limit)
        tLength -= (tLength - limit);
    }
  }

  bank[currentBank].pattern[currentPattern].track_Length[selected_Track] = tLength;

  stepSeq_page = 0;
  //update_screen = true;
  fired_1 = false;
}


void set_shift_value(byte track) {
  //byte pLength = (bank[currentBank].pattern[currentPattern].tracksRetrig) ? bank[currentBank].pattern[currentPattern].pattern_Length : get_track_Length(track, 4);
  byte pLength = get_track_Length(track, 4);

  if ((enc1_dir == -1) && (bank[currentBank].pattern[currentPattern].shiftValue[track] < pLength * 2)) {
    bank[currentBank].pattern[currentPattern].shiftValue[track] += 1;
  }

  else if ((enc1_dir == 1) && (bank[currentBank].pattern[currentPattern].shiftValue[track] > -(pLength * 2))) {
    bank[currentBank].pattern[currentPattern].shiftValue[track] -= 1;
  }

  if (bank[currentBank].pattern[currentPattern].shiftValue[track] != 0)
    bank[currentBank].pattern[currentPattern].shift_active = 1;

  else {
    for (byte track = 0; track < TRACKS; ++track) {
      if (bank[currentBank].pattern[currentPattern].shiftValue[track] != 0) {
        bank[currentBank].pattern[currentPattern].shift_active = 1;
        break;
      }
      else
        bank[currentBank].pattern[currentPattern].shift_active = 0;
    }
  }

  //update_screen = true;
  fired_1 = false;
}


void set_delete_param() {
  byte arrSize = sizeof(clear_trigsOrPitch_param) / sizeof(clear_trigsOrPitch_param[0]);
  if ((enc1_dir == 1) && (clear_trigsOrPitch_cursor < (arrSize - 1))) {
    clear_trigsOrPitch_cursor++;
  }

  else if ((enc1_dir == -1) && (clear_trigsOrPitch_cursor > 0)) {
    clear_trigsOrPitch_cursor--;
  }

  //update_screen = true;
  fired_1 = false;
}


void set_clear_param() {
  byte arrSize = sizeof(clear_SelectTrack_param) / sizeof(clear_SelectTrack_param[0]);
  if ((enc2_dir == 1) && (clear_SelectTrack_Cursor < (arrSize - 1))) {
    clear_SelectTrack_Cursor++;
  }

  else if ((enc2_dir == -1) && (clear_SelectTrack_Cursor > 0)) {
    clear_SelectTrack_Cursor--;
  }

  //update_screen = true;
  fired_2 = false;
}


void set_arp_Rate() {
  byte arrSize = sizeof(arp_Rate) / sizeof(arp_Rate[0]);

  if ((enc2_dir == 1) && (arp_Rate_Cursor < (arrSize - 1))) {
    arp_Rate_Cursor++;
  }

  else if ((enc2_dir == -1) && (arp_Rate_Cursor > 0)) {
    arp_Rate_Cursor--;
  }

  //update_screen = true;
  fired_2 = false;
}


void set_arp_Style() {
  byte arrSize = sizeof(arp_Style) / sizeof(arp_Style[0]);

  if ((enc2_dir == 1) && (arp_Style_Cursor < (arrSize - 1))) {
    arp_Style_Cursor++;
  }

  else if ((enc2_dir == -1) && (arp_Style_Cursor > 0)) {
    arp_Style_Cursor--;
  }

  //update_screen = true;
  fired_2 = false;
}


void set_Settings_selection() { // selection du param à éditer
  byte arrSize = sizeof(settings_titles) / sizeof(settings_titles[0]);

  if ((enc1_dir == 1) && (settings_titles_Cursor < (arrSize - 1))) {
    settings_titles_Cursor++;
  }

  else if ((enc1_dir == -1) && (settings_titles_Cursor > 0)) {
    settings_titles_Cursor--;
  }

  update_screen_POTS_DIAL_L = true;
  update_screen_POTS_DIAL_R = true;
  fired_1 = false;
}


void set_param_SYNC_mode() {
  if ((enc2_dir == 1) && !SYNC_mode_internal) { // ON PASSE à CLOCK INTERNE
    SYNC_mode_internal = 1;
    detachInterrupt(digitalPinToInterrupt(clockInPin));

    //CLOCK_EXT_timer.disableTimer();

    step_micros = calc_step_micros();
    CLOCK_INT_timer.setInterval(step_micros, do_MASTERClock);

    //CLOCK_INT_timer.reattachInterrupt();
  }

  else if ((enc2_dir == -1) && SYNC_mode_internal) {  // ON PASSE à CLOCK EXTERNE
    SYNC_mode_internal = 0;

    //CLOCK_INT_timer.detachInterrupt();
    CLOCK_INT_timer.disableTimer();

    //attachInterrupt(digitalPinToInterrupt(clockInPin), ISR_clock_Input, CHANGE);
    attachInterrupt(digitalPinToInterrupt(clockInPin), do_MASTERClock, FALLING);
    //CLOCK_EXT_timer.reattachInterrupt();
  }

  update_LFO_Rate_micros();
  get_RollRateCalc();

  //update_screen = true;
  fired_2 = false;
}


void set_internal_BPM() {
  if (enc2_dir == 1) {
    if (!enc2_Range_10) {
      if (bank[currentBank].pattern[currentPattern].internal_BPM < MAX_BPM) {
        bank[currentBank].pattern[currentPattern].internal_BPM++;
      }
    }

    else {
      if (bank[currentBank].pattern[currentPattern].internal_BPM <= MAX_BPM - 10) {
        bank[currentBank].pattern[currentPattern].internal_BPM += 10;
      }

      else if (bank[currentBank].pattern[currentPattern].internal_BPM < MAX_BPM) {
        bank[currentBank].pattern[currentPattern].internal_BPM += (MAX_BPM - bank[currentBank].pattern[currentPattern].internal_BPM);
      }
    }
  }

  else if (enc2_dir == -1) {
    if (!enc2_Range_10) {
      if (bank[currentBank].pattern[currentPattern].internal_BPM > MIN_BPM) {
        bank[currentBank].pattern[currentPattern].internal_BPM--;
      }
    }

    else {
      if (bank[currentBank].pattern[currentPattern].internal_BPM >= MIN_BPM + 10) {
        bank[currentBank].pattern[currentPattern].internal_BPM -= 10;
      }

      else if (bank[currentBank].pattern[currentPattern].internal_BPM > MIN_BPM) {
        bank[currentBank].pattern[currentPattern].internal_BPM -= (bank[currentBank].pattern[currentPattern].internal_BPM - MIN_BPM);
      }
    }
  }

  step_micros = calc_step_micros();
  CLOCK_INT_timer.setInterval(step_micros, do_MASTERClock);
  calc_GLIDE_time();

  update_LFO_Rate_micros();
  get_RollRateCalc();

  fired_2 = false;
}


void set_param_loopMode() {
  if (enc2_dir == 1) {
    if (bank[currentBank].pattern[currentPattern].tracksRetrig == 1) {
      bank[currentBank].pattern[currentPattern].tracksRetrig = 0;
    }
  }

  else if (enc2_dir == -1) {
    if (bank[currentBank].pattern[currentPattern].tracksRetrig == 0) {
      bank[currentBank].pattern[currentPattern].tracksRetrig = 1;
    }
  }

  fired_2 = false;
}


void set_param_firstStep() {
  //uint16_t pattern_LengthDisplay = get_pattern_Length();
  byte firstStep_increment = (enc2_Range_10) ? 10 : 1;
  //byte pLength = (bank[currentBank].pattern[currentPattern].tracksRetrig) ? bank[currentBank].pattern[currentPattern].pattern_Length : selected

  if (enc2_dir == 1) {
    if (bank[currentBank].pattern[currentPattern].firstStep <= (bank[currentBank].pattern[currentPattern].pattern_Length - 1) - firstStep_increment) {
      bank[currentBank].pattern[currentPattern].firstStep += firstStep_increment;
    }
  }

  else if (enc2_dir == -1) {
    if (bank[currentBank].pattern[currentPattern].firstStep >= firstStep_increment) {
      bank[currentBank].pattern[currentPattern].firstStep -= firstStep_increment;
    }
  }

  fired_2 = false;
}


void set_param_cvin(bool cvInNum) {
  byte arrSize = sizeof(cvIn_modulation_Target) / sizeof(cvIn_modulation_Target[0]);

  if (enc2_dir == 1) {
    if (CVin[cvInNum].dest_Cursor < (arrSize - 1)) {
      CVin[cvInNum].dest_Cursor++;
    }
  }

  else if (enc2_dir == -1) {
    if (CVin[cvInNum].dest_Cursor > 0) {
      CVin[cvInNum].dest_Cursor--;
    }
  }

  fired_2 = false;
}


void set_lfo_dest(byte lfoNum) {
  byte arrSize = sizeof(LFOs_output) / sizeof(LFOs_output[0]);

  if (enc2_dir == 1) {
    if (bank[currentBank].pattern[currentPattern].lfo_dest[lfoNum] < (arrSize - 1)) {
      bank[currentBank].pattern[currentPattern].lfo_dest[lfoNum]++;
    }
  }

  else if (enc2_dir == -1) {
    if (bank[currentBank].pattern[currentPattern].lfo_dest[lfoNum] > 0) {
      bank[currentBank].pattern[currentPattern].lfo_dest[lfoNum]--;
    }
  }

  fired_2 = false;
}


void set_lfo_wave(byte lfoNum) {
  byte arrSize = sizeof(LFOs_waves) / sizeof(LFOs_waves[0]);

  if (enc2_dir == 1) {
    if (bank[currentBank].pattern[currentPattern].lfo_wave[lfoNum] < (arrSize - 1)) {
      bank[currentBank].pattern[currentPattern].lfo_wave[lfoNum]++;
    }
  }

  else if (enc2_dir == -1) {
    if (bank[currentBank].pattern[currentPattern].lfo_wave[lfoNum] > 0) {
      bank[currentBank].pattern[currentPattern].lfo_wave[lfoNum]--;
    }
  }

  if (bank[currentBank].pattern[currentPattern].lfo_wave[lfoNum] == 4)
    LFO[0].randomVal = RandomNum.randrange(52200);

  fired_2 = false;
}


void set_lfo_rate(byte lfoNum) {
  byte arrSize = sizeof(LFOs_rates) / sizeof(LFOs_rates[0]); // arrSize = 4

  if (enc2_dir == 1) {
    if (bank[currentBank].pattern[currentPattern].lfo_rate[lfoNum] < (arrSize - 1)) {
      bank[currentBank].pattern[currentPattern].lfo_rate[lfoNum]++;
    }
  }

  else if (enc2_dir == -1) {
    if (bank[currentBank].pattern[currentPattern].lfo_rate[lfoNum] > 0) {
      bank[currentBank].pattern[currentPattern].lfo_rate[lfoNum]--;
    }
  }

  LFO[lfoNum].rate_micros = get_LFO_Rate_micros(lfoNum);

  //LFOs_enable_disable();
  update_LFO_Rate_micros();

  fired_2 = false;
}


void set_LFO_gain(byte lfoNum) {

  if (enc2_dir == 1) {
    if (enc2_Range_10 == 0) {
      if (bank[currentBank].pattern[currentPattern].lfo_gain[lfoNum] < 100) {
        bank[currentBank].pattern[currentPattern].lfo_gain[lfoNum]++;
      }
    }

    else {
      if (bank[currentBank].pattern[currentPattern].lfo_gain[lfoNum] < 91) {
        bank[currentBank].pattern[currentPattern].lfo_gain[lfoNum] += 10;
      }
    }
  }

  else if (enc2_dir == -1) {
    if (enc2_Range_10 == 0) {
      if (bank[currentBank].pattern[currentPattern].lfo_gain[lfoNum] > 1) {
        bank[currentBank].pattern[currentPattern].lfo_gain[lfoNum]--;
      }
    }
    else {
      if (bank[currentBank].pattern[currentPattern].lfo_gain[lfoNum] > 10) {
        bank[currentBank].pattern[currentPattern].lfo_gain[lfoNum] -= 10;
      }
    }
  }

  fired_2 = false;
}


void set_LFO_phase(byte lfoNum) {
  int8_t x = bank[currentBank].pattern[currentPattern].lfo_phase[lfoNum];
  if (enc2_dir == 1) {
    if (x >= -120 && x <= 110)
      x += 10;
    else if (x == 120)
      x += 7;
    else if (x == -128)
      x += 8;
  }

  else if (enc2_dir == -1) {
    if (x >= -110 && x <= 120)
      x -= 10;
    else if (x == 127)
      x -= 7;
    else if (x == -120)
      x -= 8;
  }

  bank[currentBank].pattern[currentPattern].lfo_phase[lfoNum] = x;

  fired_2 = false;
}


void set_param_replicateBars() {
  if (enc2_dir == 1) {
    if (!replicateBars) {
      replicateBars = 1;
    }
  }

  else if (enc2_dir == -1) {
    if (replicateBars) {
      replicateBars = 0;
    }
  }

  fired_2 = false;
}


void set_param_patternChange_Quantized() {
  if (enc2_dir == 1)
    patternChange_Quantized = 1;

  else if (enc2_dir == -1)
    patternChange_Quantized = 0;

  fired_2 = false;
}


void set_param_trigDur() {
  byte arrSize = sizeof(trig_durations) / sizeof(trig_durations[0]);

  if (enc2_dir == 1) {
    if (trig_durations_Cursor < (arrSize - 1)) {
      trig_durations_Cursor++;
    }
  }

  else if (enc2_dir == -1) {
    if (trig_durations_Cursor > 0) {
      trig_durations_Cursor--;
    }
  }

  trig_DURATION = trig_durations[trig_durations_Cursor];

  fired_2 = false;
}


void set_bank_patternEdit_action() {
  byte arrSize = sizeof(bank_pattern_EditAction) / sizeof(bank_pattern_EditAction[0]);

  if (enc2_dir == 1) {
    if (bank_pattern_EditAction_Cursor < (arrSize - 1)) {
      bank_pattern_EditAction_Cursor++;
    }
  }

  else if (enc2_dir == -1) {
    if (bank_pattern_EditAction_Cursor > 0) {
      bank_pattern_EditAction_Cursor--;
    }
  }

  fired_2 = false;
}


void set_songMode_Step() {
  byte arrSize = sizeof(bank[currentBank].songPatterns) / sizeof(bank[currentBank].songPatterns[0]);
  if (enc1_dir == 1) {
    if (songMode_current_Step_pot < (arrSize - 1))
      songMode_current_Step_pot++;
  }

  else if (enc1_dir == -1) {
    if (songMode_current_Step_pot > 0)
      songMode_current_Step_pot--;
  }
  
  fired_1 = false;
}


void set_led_brightness() {
  if (enc2_dir == 1) {
    if (led_brightness < 150) {
      led_brightness += 10;
    }
  }

  else if (enc2_dir == -1) {
    if (led_brightness > 10) {
      led_brightness -= 10;
    }
  }

  strip.setBrightness(led_brightness);
  strip.show();

  fired_2 = false;
}



void set_clkIn_Div() {
  if (enc2_dir == 1) {
    if (bank[currentBank].pattern[currentPattern].clkIn_Div < 8) {
      bank[currentBank].pattern[currentPattern].clkIn_Div++;
    }
  }

  else if (enc2_dir == -1) {
    if (bank[currentBank].pattern[currentPattern].clkIn_Div > 1) {
      bank[currentBank].pattern[currentPattern].clkIn_Div--;
    }
  }

  //update_screen = true;
  fired_2 = false;
}


void set_clkOut_Div() {
  if (enc2_dir == 1) {
    if (bank[currentBank].pattern[currentPattern].clkOut_Div < 8) {
      bank[currentBank].pattern[currentPattern].clkOut_Div++;
    }
  }

  else if (enc2_dir == -1) {
    if (bank[currentBank].pattern[currentPattern].clkOut_Div > 1) {
      bank[currentBank].pattern[currentPattern].clkOut_Div--;
    }
  }

  //update_screen = true;
  fired_2 = false;
}


void set_quantizeOnOff() {
  if (enc2_dir == 1) {
    if (bank[currentBank].pattern[currentPattern].quantizeBeat != 1) {
      bank[currentBank].pattern[currentPattern].quantizeBeat = 1;
    }
  }

  else if (enc2_dir == -1) {
    if (bank[currentBank].pattern[currentPattern].quantizeBeat != 0) {
      bank[currentBank].pattern[currentPattern].quantizeBeat = 0;
    }
  }

  //update_screen = true;
  fired_2 = false;
}


void set_shuffle() {
  if (enc2_dir == 1) {
    if (bank[currentBank].pattern[currentPattern].shuffle < 127) {
      bank[currentBank].pattern[currentPattern].shuffle++;
    }
  }

  else if (enc2_dir == -1) {
    if (bank[currentBank].pattern[currentPattern].shuffle > 0) {
      bank[currentBank].pattern[currentPattern].shuffle--;
    }
  }

  //update_screen = true;
  fired_2 = false;
}


void set_chromaMode_Arp_params_selection() {
  byte arrSize = sizeof(chromaMode_Arp_params_labels) / sizeof(chromaMode_Arp_params_labels[0]);

  if ((enc1_dir == 1) && (chromaMode_Arp_params < (arrSize))) {
    chromaMode_Arp_params++;
  }
  else if ((enc1_dir == -1) && (chromaMode_Arp_params > 1)) {
    chromaMode_Arp_params--;
  }

  //update_screen = true;
  fired_1 = false;
}


void set_track_type_TrigOrGate() {
  if (enc2_dir == 1) {
    bank[currentBank].pattern[currentPattern].track_isGate[selected_Track] = 1;
  }

  else if (enc2_dir == -1) {
    bank[currentBank].pattern[currentPattern].track_isGate[selected_Track] = 0;
  }

  //update_screen = true;


  fired_2 = false;
}


/*
  void set_DAC_value() { // pour mettre les bons pitchs dans l'array (à virer quand pitchs ok)
  if ((encoder2.getPosition() < encoder2_Position) && pot_value < 65300 )
    pot_value += 50;

  else if (!(encoder2.getPosition() < encoder2_Position) && pot_value > 50)
    pot_value -= 50;

  fired_2 = false;
  Dac.setValue(0, -pot_value);
  }
*/
