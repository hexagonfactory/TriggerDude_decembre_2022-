void makeStripDisplay_Nothing() {
  if (update_padLeds) {
    for (byte track = 0; track < TRACKS; ++track) {
      strip.setPixelColor(track, 0);
    }

    update_padLeds = false;
    strip.show();
  }
}


void makeStripDisplayRemote() {
  if (PLAYING) {
    if ((global_Step_Counter16 == 0) || (global_Step_Counter16 == 4) || (global_Step_Counter16 == 8) || (global_Step_Counter16 == 12))
      strip.setPixelColor(13, playBtn);
    else
      strip.setPixelColor(13, 0);
  }
  else if (PLAY_pushed) {
    strip.setPixelColor(13, playBtn);
  }

  if (recMode)
    strip.setPixelColor(14, recModeActive);
  else {
    if (MICROTIMING)
      strip.setPixelColor(14, 17, 105, 64);
    else
      strip.setPixelColor(14, 0);
  }

  if (global_Step_Counter16 == 0)
    strip.setPixelColor(12, 255, 231, 97);
  else
    strip.setPixelColor(12, 0);
}


void makeStripDisplay_Trigs(uint32_t playColor, uint32_t blankColor) {
  if (update_padLeds) {
    for (byte track = 0; track < TRACKS; ++track) {
      if (trigsIn_BitVal[track]) {
        strip.setPixelColor(track, playA);
      }
      else {
        if (trigsOut_BitVal[track])
          strip.setPixelColor(track, playColor);
        else
          strip.setPixelColor(track, blankColor);
      }
    }

    update_padLeds = false;
    strip.show();
    //update_padLeds_off = true;
  }

  /*if (update_padLeds_off) {
    if (micros() - clock_time_16PPQN >= step_micros - (step_micros / 4)) {
      update_padLeds_off = false;
      for (byte track = 0; track < TRACKS; ++track) {
        if (trigsIn_BitVal[track])
          strip.setPixelColor(track, playA);
        else if (PLAYING)
          strip.setPixelColor(track, blankColor);
      }
      strip.show();
    }
    }*/
}


void makeStripDisplay_ChromaMode() {
  if (update_padLeds) {
    for (byte pad = 0; pad < TRACKS; ++pad) {
      uint32_t color = (pad == 1 || pad == 3 || pad == 6 || pad == 8 || pad == 10) ? /*playA*/ strip.Color(40, 41, 59) : strip.Color(175, 195, 227);

      byte realTrack = selected_Track + bank[currentBank].pattern[currentPattern].AB_State[selected_Track];
      uint16_t stepToRead_TRIGS = get_track_step_TRIGS(pad, 0);
      uint8_t byteToRead_TRIGS = stepToRead_TRIGS / 8;
      uint8_t bitToRead_TRIGS = stepToRead_TRIGS & 7;

      if (trigsIn_BitVal[pad]
          || ((bank[currentBank].pattern[currentPattern].Pitch[realTrack][global_Step_4PPQN] % 12 == pad) && ((bank[currentBank].pattern[currentPattern].Trigs[realTrack][byteToRead_TRIGS] & masks[bitToRead_TRIGS]) > 0)))
        strip.setPixelColor(pad, 250, 250, 250);
      else
        strip.setPixelColor(pad, color);
    }

    update_padLeds = false;
    strip.show();
  }
}


void makeStripDisplay_ChromaMode_Arp() {
  if (update_padLeds) {
    for (byte pad = 0; pad < TRACKS; ++pad) {
      int32_t color = (pad == 1 || pad == 3 || pad == 6 || pad == 8 || pad == 10) ? /*playA*/ strip.Color(40, 41, 59) : strip.Color(175, 195, 227);

      byte realTrack = selected_Track + bank[currentBank].pattern[currentPattern].AB_State[selected_Track];
      uint16_t stepToRead_TRIGS = get_track_step_TRIGS(pad, 0);
      uint8_t byteToRead_TRIGS = stepToRead_TRIGS / 8;
      uint8_t bitToRead_TRIGS = stepToRead_TRIGS & 7;

      if (trigsIn_BitVal[pad]
          || ((bank[currentBank].pattern[currentPattern].Pitch[realTrack][global_Step_4PPQN] % 12 == pad) && ((bank[currentBank].pattern[currentPattern].Trigs[realTrack][byteToRead_TRIGS] & masks[bitToRead_TRIGS]) > 0)))
        strip.setPixelColor(pad, 250, 250, 250);
      else
        strip.setPixelColor(pad, color);
    }

    update_padLeds = false;
    strip.show();
  }
}


void makeStripDisplay_ChromaMode_Transpose() {
  if (update_padLeds) {
    for (byte track = 0; track < TRACKS; ++track) {
      if (track < 8) {
        byte realTrack = selected_Track + bank[currentBank].pattern[currentPattern].AB_State[selected_Track];
        if (track == bank[currentBank].pattern[currentPattern].transposeValue[realTrack] + 2) {
          strip.setPixelColor(track, 0, 219, 100);
        }

        else if (track == 2) {
          strip.setPixelColor(track, playA);
        }

        else {
          strip.setPixelColor(track, 182, 69, 196);
        }
      }

      else {
        strip.setPixelColor(track, 0);
      }
    }

    update_padLeds = false;
    strip.show();
  }
}


void makeStripDisplay_ChromaMode_CVoutAssign() {
  if (update_padLeds) {
    for (byte track = 0; track < TRACKS; ++track) {
      if (track < 4)
        makeStripDisplay_CVOuts(track, selected_Track);
      else
        strip.setPixelColor(track, 0);
    }

    update_padLeds = false;
    strip.show();
  }
}


void makeStripDisplay_AB_State() {
  if (update_padLeds) {
    uint32_t playC = strip.Color(255, 217, 0);// AB mode B background
    for ( byte track = 0; track < TRACKS; ++track) {
      if (bank[currentBank].pattern[currentPattern].AB_State[track] == 12) strip.setPixelColor(track, 17, 140, 201);
      else strip.setPixelColor(track, playC);
    }

    update_padLeds = false;
    strip.show();
  }
}


void makeStripDisplay_TrackLength() {
  if (update_padLeds) {
    for (byte pad = 0; pad < TRACKS; ++pad) {
      if (pad == selected_Track)
        strip.setPixelColor(pad, 17, 140, 201);
      else
        strip.setPixelColor(pad, 0);
    }

    update_padLeds = false;
    strip.show();
  }

}


void makeStripDisplay_Mute_State() {
  if (update_padLeds) {
    for ( byte track = 0; track < TRACKS; ++track) {
      //if (bank[currentBank].pattern[currentPattern].muteState[track] == 0) strip.setPixelColor(track, 0, 219, 100);
      //else strip.setPixelColor(track, 219, 0, 37);
      uint8_t byteToRead = track / 8;
      uint8_t bitPos = track & 7;
      if ((bank[currentBank].pattern[currentPattern].mute_solo_States[byteToRead] & masks[bitPos]) > 0)
        strip.setPixelColor(track, 219, 0, 37);
      else
        strip.setPixelColor(track, 0, 219, 100);
    }

    update_padLeds = false;
    strip.show();
  }
}


void makeStripDisplay_Solo_State() {
  if (update_padLeds) {
    uint32_t playG1 = strip.Color(45, 101, 168);// Solo background
    uint32_t playG = strip.Color(255, 217, 0);// Solo - soloed

    for ( byte track = 0; track < TRACKS; ++track) {
      uint8_t byteToRead_solo = (track + 12) / 8;
      uint8_t bitPos_solo = (track + 12) & 7;
      if ((bank[currentBank].pattern[currentPattern].mute_solo_States[byteToRead_solo] & masks[bitPos_solo]) > 0)
        strip.setPixelColor(track, playG);
      else
        strip.setPixelColor(track, playG1);
    }

    update_padLeds = false;
    strip.show();
  }
}


void makeStripDisplay_Fill_State() {
  if (update_padLeds) {
    for ( byte track = 0; track < TRACKS; ++track) {
      uint8_t byteToRead = track / 8;
      uint8_t bitPos = track & 7;
      if ((bank[currentBank].pattern[currentPattern].fillState[byteToRead] & masks[bitPos]) > 0)
        strip.setPixelColor(track, playB);
      else
        strip.setPixelColor(track, playA);
    }

    update_padLeds = false;
    strip.show();
  }
}


void makeStripDisplay_Shift_Value() {
  if (update_padLeds) {
    byte currentBankDisplay = get_currentBank_Display(0);
    byte currentPatternDisplay = get_currentPattern_Display(0);
    for ( byte track = 0; track < TRACKS; ++track) {
      int8_t shiftVal = bank[currentBankDisplay].pattern[currentPatternDisplay].shiftValue[track];
      byte x = track + bank[currentBankDisplay].pattern[currentPatternDisplay].AB_State[track];
      uint8_t byteToRead = x / 8;
      uint8_t bitPos = x & 7;
      //if (bank[currentBankDisplay].pattern[currentPatternDisplay].track_has_Trigs[x] == 1) {
      if ((bank[currentBankDisplay].pattern[currentPatternDisplay].track_has_Trigs[byteToRead] & masks[bitPos]) > 0) {
        if (shiftVal == 0) {
          strip.setPixelColor(track, 250, 250, 250);
        }

        else {
          if (shiftVal > 0) {
            strip.setPixelColor(track, 100, 100, 250);
          }

          else {
            strip.setPixelColor(track, 250, 100, 100);
          }
        }

        if ((global_Step_Counter16 == 0) || (global_Step_Counter16 == 4) || (global_Step_Counter16 == 8) || (global_Step_Counter16 == 12)) {
          strip.setPixelColor(selected_Track, 0);
        }
      }
      else
        strip.setPixelColor(track, 0);
    }

    update_padLeds = false;
    strip.show();
  }
}


void makeStripDisplay_Clear_Screen() {
  if (update_padLeds) {
    byte currentBankDisplay = get_currentBank_Display(0);
    byte currentPatternDisplay = get_currentPattern_Display(0);

    for ( byte track = 0; track < TRACKS; ++track) {
      byte realTrack = track + bank[currentBankDisplay].pattern[currentPatternDisplay].AB_State[track];
      uint8_t byteToRead = realTrack / 8;
      uint8_t bitPos = realTrack & 7;
      //if (bank[currentBankDisplay].pattern[currentPatternDisplay].track_has_Trigs[track + bank[currentBankDisplay].pattern[currentPatternDisplay].AB_State[track]] == 1) {
      if ((bank[currentBankDisplay].pattern[currentPatternDisplay].track_has_Trigs[byteToRead] & masks[bitPos]) > 0) {
        if (trigsOut_BitVal[track] == 1) {
          strip.setPixelColor(track, recModeActive);
        }
        else {
          if (bank[currentBankDisplay].pattern[currentPatternDisplay].AB_State[track] == 12)
            strip.setPixelColor(track, 17, 140, 201);
          else
            strip.setPixelColor(track, 255, 217, 0);
        }
      }
      else
        strip.setPixelColor(track, 0);
    }

    update_padLeds = false;
    strip.show();
  }
}


void makeStripDisplay_Pattern_Selection() {
  if (update_padLeds) {
    byte currentPatternDisplay = get_currentPattern_Display(0);
    for (byte pad = 0; pad < PATTERNS; ++pad) {
      if (pad == currentPatternDisplay)
        strip.setPixelColor(pad, playD);

      else {
        if (bank[currentBank].notEmpty_Pattern[pad] == 1)
          strip.setPixelColor(pad, playE);
        else
          strip.setPixelColor(pad, 0);
      }
    }

    for (byte pad = 8; pad < 12; ++pad) {  // A ENLEVER QUAND STOCKAGE OK POUR 12 PATTERNS
      strip.setPixelColor(pad, 0);
    }

    update_padLeds = false;
    strip.show();
  }
}


void makeStripDisplay_Pattern_EditMode() {
  if (update_padLeds) {
    for (byte pad = 0; pad < PATTERNS; ++pad) {
      if (patternSelected == 255) {
        if (bank[currentBank].notEmpty_Pattern[pad] == 1)
          strip.setPixelColor(pad, playD);
        else
          strip.setPixelColor(pad, 0);
      }
      else {
        if (!patternToEdit_readyToAction) {
          if (pad == patternSelected)
            strip.setPixelColor(pad, 219, 197, 0);
          else
            strip.setPixelColor(pad, 0);
        }

        else if (bank_pattern_EditAction_Cursor == 2) {
          if (pad == patternDestination)
            strip.setPixelColor(pad, playA);
          else
            strip.setPixelColor(pad, playD);
        }
      }

      if (pad > 8)
        strip.setPixelColor(pad, 0);
    }

    update_padLeds = false;
    strip.show();
  }
}


void makeStripDisplay_Bank_EditMode() {
  if (update_padLeds) {
    for (byte pad = 0; pad < BANKS; ++pad) {
      if (bankSelected == 255) {
        if (bank[currentBank].notEmpty_Pattern[pad] == 1)
          strip.setPixelColor(pad, playD);
        else
          strip.setPixelColor(pad, 0);
      }
      else {
        if (!bankToEdit_readyToAction) {
          if (pad == bankSelected)
            strip.setPixelColor(pad, 219, 197, 0);
          else
            strip.setPixelColor(pad, 0);
        }

        else if (bank_pattern_EditAction_Cursor == 2) {
          if (pad == bankDestination)
            strip.setPixelColor(pad, playA);
          else
            strip.setPixelColor(pad, playD);
        }
      }

      if (pad > 8)
        strip.setPixelColor(pad, 0);
    }

    update_padLeds = false;
    strip.show();
  }
}


void makeStripDisplay_SongMode() {
  if (update_padLeds) {
    for (byte pad = 0; pad < PATTERNS; ++pad) {
      if (pad == bank[currentBank].songPatterns[songMode_current_Step_PLAYING])
        strip.setPixelColor(pad, playD);

      else {
        if (bank[currentBank].notEmpty_Pattern[pad] == 1)
          strip.setPixelColor(pad, playE);
        else
          strip.setPixelColor(pad, 0);
      }
    }

    update_padLeds = false;
    strip.show();
  }
}


void makeStripDisplay_Bank_Selection() {
  if (update_padLeds) {
    byte currentBankDisplay = get_currentBank_Display(0);
    for (byte pad = 0; pad < TRACKS; ++pad) {
      if (pad == currentBankDisplay)
        strip.setPixelColor(pad, 219, 197, 0);
      else
        strip.setPixelColor(pad, playE);
    }

    update_padLeds = false;
    strip.show();
  }
}


void makeStripDisplay_StepSeq(byte selected_Track) {
  if (update_padLeds) {
    uint32_t stepSeqBlank = strip.Color(220, 125, 30); // step seq blank step
    uint32_t color = strip.Color(0, 0, 0);

    //byte currentBankDisplay = get_currentBank_Display(0);
    //byte currentPatternDisplay = get_currentPattern_Display(0);

    byte realTrack = selected_Track + step_seq_ab[selected_Track];
    uint8_t byteToRead = 0;
    uint16_t stepSeq_page_16 = stepSeq_page * 4; // 32

    for (byte pad = 0; pad < TRACKS; ++pad) {
      if (pad < 4) {
        if (!MICROTIMING || stepSeq_clickedStep == 255)
          strip.setPixelColor(pad, 0, 0, 0);

        else {
          color = ((bank[currentBank].pattern[currentPattern].Trigs[realTrack][selectedSeqStep_byte] & masks[selectedSeqStep_bitPos + pad]) > 0) ? playD : stepSeqBlank;
          strip.setPixelColor(pad, color);

          /*if (global_Step_4PPQN == selectedSeqStep_byte && global_Step_16PPQN == selectedSeqStep_bitPos + pad)
            strip.setPixelColor(pad, playB);*/
        }
      }

      else {
        uint16_t stepSeq_global_Step_16PPQN = (pad < 8) ? (pad * 4) + stepSeq_page_16 : ((pad - 8) * 4) + stepSeq_page_16;
        byteToRead = stepSeq_global_Step_16PPQN / 8;

        bool padHasTrigs = false;
        bool padHasTrigs_decal = false;
        static byte index = 0;
        static byte limit = 4;

        if (pad == 4 || pad == 6 || pad == 8 || pad == 10) {
          index = 0;
          limit = 4;
        }
        else {
          index = 4;
          limit = 8;
        }

        for (byte i = index; i < limit; ++i) {
          if ((bank[currentBank].pattern[currentPattern].Trigs[realTrack][byteToRead] & masks[i]) > 0) {
            padHasTrigs = true;

            if (i != index)
              padHasTrigs_decal = true;

            /*if (!MICROTIMING)
              break;*/
          }
        }

        byte current_Step_tracker = (pad < 8) ? (track_Step_16PPQN[selected_Track] / 4) - stepSeq_page : ((track_Step_16PPQN[selected_Track] / 4) - stepSeq_page) + 8;

        color = (stepSeq_global_Step_16PPQN < get_track_Length(selected_Track, 16) - 1) ?
                ((padHasTrigs) ? ((padHasTrigs_decal ? strip.Color(40, 121, 168) : playA)) : strip.Color(219, 190, 94) ) : strip.Color(0, 0, 0);

        strip.setPixelColor(pad, color);

        if (PLAYING && (track_Step_16PPQN[selected_Track] / 4) == stepSeq_global_Step_16PPQN / 4)
          strip.setPixelColor(current_Step_tracker, playB);

        if (stepSeq_clickedStep != 255)
          strip.setPixelColor(stepSeq_clickedStep, playB);
      }
    }

    update_padLeds = false;
    strip.show();
  }
}


void makeStripDisplay_CVOuts(byte padID, byte track) {
  if (bank[currentBank].pattern[currentPattern].cvOut_Tracks[padID] == track)
    strip.setPixelColor(padID, playD);
  else if (bank[currentBank].pattern[currentPattern].cvOut_Tracks[padID] == 12)
    strip.setPixelColor(padID, stepSeqCvOutBlank);
  else
    strip.setPixelColor(padID, 219, 0, 37);
}


void makeStripDisplay_PitchSeq(byte selected_Track, byte step_clicked) {
  if (update_padLeds) {
    uint32_t color = strip.Color(0, 0, 0);

    byte realTrack = selected_Track + step_seq_ab[selected_Track];
    uint8_t byteToRead = 0;
    uint16_t stepSeq_page_16 = stepSeq_page * 4; // 32


    for (byte pad = 0; pad < TRACKS; ++pad) {
      if (pad < 4) {
        makeStripDisplay_CVOuts(pad, selected_Track);
      }

      else {
        uint16_t stepSeq_global_Step_16PPQN = (pad < 8) ? (pad * 4) + stepSeq_page_16 : ((pad - 8) * 4) + stepSeq_page_16;
        byteToRead = stepSeq_global_Step_16PPQN / 8;

        bool padHasTrigs = false;
        bool padHasTrigs_decal = false;
        byte index = 0;
        byte limit = 4;

        if (pad == 5 || pad == 7 || pad == 9 || pad == 11) {
          index = 4;
          limit = 8;
        }

        for (byte i = index; i < limit; ++i) {
          if ((bank[currentBank].pattern[currentPattern].Trigs[realTrack][byteToRead] & masks[i]) > 0) {
            padHasTrigs = true;
            if (i != index)
              padHasTrigs_decal = true;
            //break;
          }
        }

        byte current_Step_tracker = (pad < 8) ? global_Step_4PPQN - stepSeq_page : (global_Step_4PPQN - stepSeq_page) + 8; // curseur du global_Step_4PPQN, qui se déplace

        color = (stepSeq_global_Step_16PPQN < get_pattern_Length(16) - 1) ?
                ((padHasTrigs) ? ((padHasTrigs_decal ? strip.Color(0, 219, 100) : playA)) : strip.Color(220, 125, 30)) : strip.Color(0, 0, 0);

        strip.setPixelColor(pad, color);

        if (PLAYING && global_Step_4PPQN == stepSeq_global_Step_16PPQN / 4)
          strip.setPixelColor(current_Step_tracker, playB);

        if (stepSeq_clickedStep != 255)
          strip.setPixelColor(step_clicked, 255, 0, 0);
      }
    }

    update_padLeds = false;
    strip.show();
  }
}
