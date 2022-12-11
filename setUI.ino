void set_UI_Config() {
  catch_Mode_Selection_Change();
  detect_remoteBtnsPush();
  poll_encoders();

  switch (screenConfig) {
    case 1: // MAIN SCREEN
      setPadsActions_normalMode();

      if (!SYNC_mode_internal)
        get_Bpm();

      if (enc1_clk)
        detect_rollOnOff();

      makeStripDisplay_Trigs(playE, 0);

      if (fired_1)
        set_rollRate();

      if (!ALT_pushed && enc2_clk) {
        detect_lockpattern_LengthOnOff();
      }

      if (fired_2) {
        if (!ALT_pushed) {
          if ( !((CVin[0].enable && CVin[0].dest_Selected == 2) || (CVin[1].enable && CVin[1].dest_Selected == 2)) ) {
            set_pattern_Length();
          }
        }

        else
          set_track_type_TrigOrGate();
      }

      drawPage_mainScreen();
      break;

    case 2: // PATTERN SELECTION
      setPadsActions_bankPatternSel();

      makeStripDisplay_Pattern_Selection();

      /*if (enc1_clk)
        detect_patternEditMode_OnOff();*/

      drawPage_Pattern_Selection();

      if (enc2_clk)
        detect_songMode_Screen();

      if (enc1_clk)
        detect_patternEditMode_OnOff();
      break;

    case 3: // A/B
      setPadsActions_ABState();

      makeStripDisplay_AB_State();

      if (enc1_clk)
        detect_resetABStates();

      if (enc2_clk)
        detect_swapABStates();

      if (fired_1)
        set_allAorB();

      drawPage_ABstate();
      break;

    case 4: // MUTE
      setPadsActions_muteSolo();

      makeStripDisplay_Mute_State();

      if (bank[currentBank].pattern[currentPattern].mute_active) {
        if (enc1_clk)
          detect_resetMuteStates();
      }

      if (!bank[currentBank].pattern[currentPattern].allMute_active) {
        if (enc2_clk)
          detect_muteAll();
      }

      drawPage_muteState();
      break;

    case 5: // FILL
      setPadsActions_Fill();

      makeStripDisplay_Fill_State();

      if (fired_1)
        set_track_FillRate();

      if (bank[currentBank].pattern[currentPattern].fill_active) {
        if (enc2_clk)
          detect_resetFillStates();

        if (!ALT_pushed && recMode && PLAY_pushed)
          recordFills();
      }

      drawPage_fillState();
      break;

    case 6: // DELETE STEPS
      setPadsActions_deleteSteps();

      makeStripDisplay_Trigs(recModeActive, 0);

      if (fired_1)
        set_delete_param();

      if (fired_2)
        set_clear_param();

      if (Trigs_Undo_enable || Pitch_Undo_enable) {
        if (enc2_clk)
          detect_undoCall();
      }

      drawPage_deleteSteps();
      break;

    case 7: // SETTINGS SCREEN
      makeStripDisplay_Nothing();

      mainSettings_encoders_events();

      drawPage_Settings();
      break;

    case 8: // ROLL MODE
      setPadsActions_rollMode();

      if (!SYNC_mode_internal)
        get_Bpm();

      get_RollRateCalc();

      //drawPage_mainScreen();
      if (enc1_clk)
        detect_rollOnOff();

      makeStripDisplay_Trigs(stepSeqCvOutBlank, 0);

      if (fired_1)
        set_rollRate();

      /*if ( !((CVin[0].enable && (CVin[0].dest_Selected) == 2) || (CVin[1].enable && (CVin[1].dest_Selected) == 2)) ) {
        detect_lockpattern_LengthOnOff();
        if (fired_2)
          set_pattern_Length();
        }*/
      drawPage_mainScreen();
      break;

    case 9: // PATTERN/LOOP LENGTH
      setPadsActions_trackLength();

      makeStripDisplay_TrackLength();

      if (enc1_clk)
        detect_encoder1_Range();

      if (fired_1)
        set_track_Length();

      if (bank[currentBank].pattern[currentPattern].tracksRetrig) {
        if ( !((CVin[0].enable && CVin[0].dest_Selected == 2) || (CVin[1].enable && CVin[1].dest_Selected == 2)) ) {
          if (enc2_clk)
            detect_lockpattern_LengthOnOff();

          if (fired_2)
            set_pattern_Length();
        }
      }

      drawPage_loopLength();
      break;

    case 10: // STEP SEQ TRIGS
      setPadsActions_stepSeq();

      makeStripDisplay_StepSeq(selected_Track);

      if (enc1_clk)
        detect_Track_AB_change(selected_Track);

      if (fired_1)
        set_selected_Track();

      if (fired_2)
        set_stepSeq_page();

      drawPage_stepSequencer();
      break;

    case 11: // STEP SEQ TRIGS - PARAMS
      setPadsActions_stepSeq();

      makeStripDisplay_StepSeq(selected_Track);

      if (stepSeq_clickedStep != 255) {
        if (enc2_clk)
          detect_stepSeq_params_OnOff();
      }

      /*if (fired_1)
        set_select_StepSeq_param();
        if (fired_2)
        set_value_StepSeq_param();*/
      drawPage_stepSequencer_params();
      break;

    case 12: // PITCH SEQUENCER
      setPadsActions_pitchSeq();

      makeStripDisplay_PitchSeq(selected_Track, stepSeq_clickedStep);

      if (enc1_clk)
        detect_encoder1_pitch_Range();

      if (stepSeq_clickedStep != 255) {
        if (fired_1)
          set_Step_pitch(selected_Track, stepSeq_global_Step_4PPQN);
      }

      if (fired_2)
        set_stepSeq_page();

      drawPage_pitchSequencer();
      break;

    case 13: // PATTERN EDIT MODE
      setPadsActions_patternEditMode();

      makeStripDisplay_Pattern_EditMode();

      //drawPage_patternEditMode();

      if (!patternToEdit_readyToAction) {
        if (patternSelected == 255) {  // si pas encore selectionne le pattern
          if (enc1_clk) {
            detect_patternEditMode_OnOff();
            break;
          }
        }

        else {
          if (fired_2)
            set_bank_patternEdit_action(); // selectionne l'action (Save / Copy / Erase)

          if (enc1_clk)
            detect_patternEditMode_Reset();

          if (enc2_clk)
            detect_patternSelected_Edit();         // click = passe à l'action selectionnée
        }
      }

      else {
        if (enc1_clk)
          detect_patternEditMode_Reset();

        if (enc2_clk) {
          if (bank_pattern_EditAction_Cursor == 0) {
            detect_reload_pattern(currentBank, currentPattern);
          }
          else if (bank_pattern_EditAction_Cursor == 1) {
            detect_save_pattern();
          }
          else if (bank_pattern_EditAction_Cursor == 2) {
            if (patternDestination != 255)
              detect_patternPaste();
          }
          else if (bank_pattern_EditAction_Cursor == 3) {
            detect_patternErase();
          }
        }
      }

      drawPage_patternEditMode();
      break;

    case 14: // SONG MODE
      setPadsActions_songMode();

      makeStripDisplay_SongMode();

      if (enc1_clk)
        detect_songMode_OnOff();

      if (enc2_clk)
        detect_songMode_Screen();

      if (fired_1)
        set_songMode_Step(); // selectionne le step à éditer dans Song mode

      drawPage_songMode();
      break;

    case 15: // BANK EDIT MODE
      setPadsActions_bankEditMode();

      makeStripDisplay_Bank_EditMode();

      if (enc1_clk)
        detect_bankEditMode_OnOff();

      if (!bankToEdit_readyToAction) {
        if (bankSelected != 255) {
          if (fired_2)
            set_bank_patternEdit_action(); // selectionne l'action (Save / Copy / Erase)
          detect_bankSelected_Edit();         // click = passe à l'action selectionnée
        }
      }
      else {
        if (enc1_clk)
          detect_patternEditMode_Reset();  //detect_bankEditMode_Reset();

        if (enc2_clk) {
          if (bank_pattern_EditAction_Cursor == 0) {
            detect_reload_pattern(currentBank, patternSelected);  //detect_reload_bank(currentBank);
          }
          else if (bank_pattern_EditAction_Cursor == 1) {
            detect_save_pattern();  //detect_save_bank();
          }
          else if (bank_pattern_EditAction_Cursor == 2) {
            if (patternDestination != 255)
              detect_patternPaste();  //detect_bankPaste();
          }
          else if (bank_pattern_EditAction_Cursor == 3) {
            detect_patternErase();  //detect_bankErase();
          }
        }
      }

      drawPage_bankEditMode();
      break;

    case 16: // PITCH GLIDE MODE
      if (enc1_clk)
        detect_GlideMode_OnOff_seq();

      if (fired_1)
        set_glide_STEPS();

      drawPage_glideMode();
      break;

    case 22: // BANK SELECTION
      setPadsActions_bankPatternSel();

      makeStripDisplay_Bank_Selection();

      if (enc1_clk)
        detect_bankEditMode_OnOff();

      drawPage_bankSelection();
      break;

    case 33: // CHROMA_MODE KBOARD
      setPadsActions_ChromaMode_KBoard();

      makeStripDisplay_ChromaMode();

      if (enc1_clk)
        detect_GlideMode_OnOff_chroma();

      if (Trigs_Undo_enable || Pitch_Undo_enable) {
        if (enc2_clk)
          detect_undoCall();
      }

      if (fired_1)
        set_ChromaMode_octave();

      drawPage_chromaMode_KBoard();

      if (fired_2)
        set_ChromaMode_UI();

      //drawPage_chromaMode_KBoard();
      break;

    case 34: // CHROMA_MODE ARP
      setPadsActions_ChromaMode_Arp();

      makeStripDisplay_ChromaMode_Arp();

      if (enc1_clk)
        detect_ARP_params_onOff();

      if (enc2_clk)
        detect_ARP_latch_onOff();

      /*if (Trigs_Undo_enable || Pitch_Undo_enable)
        detect_undoCall();*/

      if (fired_1) {
        if (chromaMode_Arp_params == 0) { // PAS DANS PARAMS
          set_ChromaMode_octave();
        }

        else { // ARP PARAMS : RATE ou STYLE
          set_chromaMode_Arp_params_selection();
        }
      }


      if (fired_2) {
        switch (chromaMode_Arp_params) {
          case 0 :
            set_ChromaMode_UI();
            break;

          case 1:
            set_arp_Rate();
            break;

          case 2:
            set_arp_Style();
            break;
        }
      }

      drawPage_chromaMode_Arp();
      break;

    case 35: // CHROMA_MODE TRANSPOSE
      setPadsActions_ChromaMode_Transpose();

      makeStripDisplay_ChromaMode_Transpose();

      if (enc1_clk)
        detect_ChromaMode_transpose_reset();

      if (fired_1)
        set_ChromaMode_transpose();

      if (fired_2)
        set_ChromaMode_UI();

      drawPage_chromaMode_Transpose();
      break;

    case 36: // CHROMA_MODE GLIDE
      makeStripDisplay_Nothing();

      if (enc1_clk)
        detect_GLIDE_paramSelection_onOff();

      if (fired_1) {
        if (!GLIDE_paramSelection_active)
          set_ChromaMode_glide_param_selection();
        else
          (GLIDE_paramSelection_cursor == 0) ? set_glide_MODE() : set_glide_STEPS();
      }

      drawPage_chromaMode_Glide();

      if (fired_2)
        set_ChromaMode_UI();

      //drawPage_chromaMode_Glide();
      break;

    case 37: // CHROMA_MODE CVOUTASSIGN
      if (!trigger_cvOutReassign_process)
        setPadsActions_ChromaMode_CVoutAssign();
      else
        detect_cvOutReassign(cvOutputPad, selected_Track);

      makeStripDisplay_ChromaMode_CVoutAssign();

      drawPage_chromaMode_CVOutSelect();

      if (fired_2)
        set_ChromaMode_UI();

      //drawPage_chromaMode_CVOutSelect();
      break;

    case 44: // SOLO
      setPadsActions_muteSolo();

      makeStripDisplay_Solo_State();

      if (bank[currentBank].pattern[currentPattern].solo_active) {
        if (enc1_clk)
          detect_resetSoloStates();
      }

      drawPage_soloState();
      break;

    case 55: // SHIFT
      setPadsActions_Shift();

      makeStripDisplay_Shift_Value();

      if (fired_1)
        set_shift_value(selected_Track);

      if (bank[currentBank].pattern[currentPattern].shiftValue[selected_Track] != 0) {
        if (enc1_clk)
          detect_clear_Track_Shift();
      }

      if (bank[currentBank].pattern[currentPattern].shift_active) {
        if (enc2_clk)
          detect_clearAllShift();
      }

      drawPage_shiftSteps();
      break;

    case 66: // CLEAR TRACK
      setPadsActions_clearTrack(); update_screen_PADS = true;

      makeStripDisplay_Clear_Screen();

      if (fired_1)
        set_delete_param();

      if (fired_2)
        set_clear_param();

      if (Trigs_Undo_enable || Pitch_Undo_enable) {
        if (enc2_clk)
          detect_undoCall();
      }

      drawPage_clearTrack();
      break;
  }

  //last_encoder1_Btn_State = 1;
  //last_encoder2_Btn_State = 1;
  //enc1_clk = false;
  //enc2_clk = false;
}




void mainSettings_encoders_events() {
  if (fired_1)
    set_Settings_selection();

  if (enc1_clk)
    detect_Settings_exit();

  switch (settings_titles_Cursor) {
    case 0: // LOOP MODE
      if (fired_2)
        set_param_loopMode();
      break;

    case 1: // FirstStep
      if (bank[currentBank].pattern[currentPattern].tracksRetrig) {
        if (enc2_clk)
          detect_encoder2_Range();

        if (fired_2)
          set_param_firstStep();
      }
      break;

    case 2: // CVin_1
      if (fired_2)
        set_param_cvin(0);

      if (enc2_clk)
        detect_param_cvMod_OnOff(0);
      break;

    case 3: // CVin_2
      if (fired_2)
        set_param_cvin(1);

      if (enc2_clk)
        detect_param_cvMod_OnOff(1);
      break;

    case 4: // replicateBars
      if (fired_2)
        set_param_replicateBars();
      break;

    case 5: // SYNC Mode
      if (fired_2)
        set_param_SYNC_mode();
      break;

    case 6: // set BPM
      if (SYNC_mode_internal) {

        if (enc2_clk)
          detect_encoder2_Range();

        if (fired_2)
          set_internal_BPM();
      }
      break;

    case 7: // set CLOCK IN DIV
      if (fired_2)
        set_clkIn_Div();
      break;

    case 8: // set CLOCK OUT DIV
      if (fired_2)
        set_clkOut_Div();
      break;

    case 9: // QUANTIZE
      if (fired_2)
        set_quantizeOnOff();
      break;

    case 10: // SHUFFLE
      if (fired_2)
        set_shuffle();
      break;

    case 11: // set LFO 1 DEST
      if (fired_2)
        set_lfo_dest(0);

      if (enc2_clk)
        detect_lfo_onOff(0);
      break;

    case 12: // set LFO 1 WAVE
      if (fired_2)
        set_lfo_wave(0);
      break;

    case 13: // set LFO 1 RATE
      if (fired_2)
        set_lfo_rate(0);
      break;

    case 14: // set LFO 1 GAIN
      if (fired_2)
        set_LFO_gain(0);

      if (enc2_clk)
        detect_encoder2_Range();
      break;

    case 15: // set LFO 1 PHASE
      if (fired_2)
        set_LFO_phase(0);

      if (bank[currentBank].pattern[currentPattern].lfo_phase[0] != 0) {
        if (enc2_clk)
          detect_reset_LFO_PHASE(0);
      }
      break;

    case 16: // set LFO 2 DEST
      if (fired_2)
        set_lfo_dest(1);

      if (enc2_clk)
        detect_lfo_onOff(1);
      break;

    case 17: // set LFO 2 WAVE
      if (fired_2)
        set_lfo_wave(1);
      break;

    case 18: // set LFO 2 RATE
      if (fired_2)
        set_lfo_rate(1);
      break;

    case 19: // set LFO 2 GAIN
      if (fired_2)
        set_LFO_gain(1);

      if (enc2_clk)
        detect_encoder2_Range();
      break;

    case 20: // set LFO 2 PHASE
      if (fired_2)
        set_LFO_phase(1);

      if (bank[currentBank].pattern[currentPattern].lfo_phase[1] != 0) {
        if (enc2_clk)
          detect_reset_LFO_PHASE(1);
      }
      break;

    case 21: // LED BRIGHTNESS
      if (fired_2)
        set_param_patternChange_Quantized();
      break;

    case 22: // TRIG DURATION
      if (fired_2)
        set_param_trigDur();
      break;

    case 23: // LED BRIGHTNESS
      if (fired_2)
        set_led_brightness();
      break;
  }
}
