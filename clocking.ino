
void LFO_1_tick() {
  LFO[0].counter++;

  if (LFO[0].counter >= LFO_WAVE_RESOLUTION) {
    LFO[0].counter = 0;                       // TESTER METTRE APRES INSTRUCTION SUIVANTE
    if (get_LFO_wave_Display(0) == 4)
      LFO[0].randomVal = RandomNum.randrange(52200);
  }
}


void LFO_2_tick() {
  LFO[1].counter++;

  if (LFO[1].counter >= LFO_WAVE_RESOLUTION) {
    LFO[1].counter = 0;
    if (get_LFO_wave_Display(1) == 4)
      LFO[1].randomVal = RandomNum.randrange(52200);
  }
}


void increment_arpCounter() {
  if (global_Step_16PPQN % arp_Rate[arp_Rate_Cursor] == 0) {
    arp_Time = micros();
    arpCounter++;
    timeFlag_arp = true;
  }
}


unsigned long calc_step_micros() {
  //step_micros = round(3750000 / bank[currentBank].pattern[currentPattern].internal_BPM);
  step_micros = round(3750000.0 / bank[currentBank].pattern[currentPattern].internal_BPM);
  return step_micros;
}


void do_MASTERClock() {
  clockPulse = LOW;
}


void do_GLIDEClock() {
  glidePulse = LOW;
}



void clockBeats() {
  if (clockPulse == LOW) {
    clock_time_16PPQN = micros();
    clockPulse = HIGH;

    if (!SYNC_mode_internal) {
      step_micros = clock_time_16PPQN - last_clock_time_16PPQN;
    }

    last_clock_time_16PPQN = clock_time_16PPQN;
    last_global_Step_16PPQN = global_Step_16PPQN;

    if (PLAY_pushed) {
      global_Step_16PPQN++;

      for (byte track = 0; track < TRACKS; ++track) {
        track_Step_16PPQN[track]++;
        if (track_Step_16PPQN[track] >= get_track_Length(track, 16))
          track_Step_16PPQN[track] = (PLAYING) ? get_firstStep(16) : get_track_Length(track, 16) - 1;
      }

      if (global_Step_16PPQN % 4 == 0) {
        last_global_Step_4PPQN = global_Step_4PPQN;
        global_Step_4PPQN++;
        global_Step_Counter16++;

        timeFlag_glide = true; // on scan currentPitch et NextPitch pour glide;
        update_screen_TOP = true;
      }

      increment_arpCounter();

      manage_Reset_Things();


      if (!SYNC_mode_internal)
        PLAYING = 1;

      displayFirstStep_1 = 0;


      if (global_Step_4PPQN != last_global_Step_4PPQN) {
        manage_changeOn_FIRSTSTEP_things();
      }

      update_shiftRegisters = true;
    }

    if (CVin[0].enable || CVin[1].enable)
      modulateParams(CVin[0].dest_Selected, CVin[1].dest_Selected);

    if (songMode_Activated)
      sequencer_play_song();

    update_padLeds = true;
  }
}


void get_RollRateCalc() {
  //rollRate_micros = (unsigned long)round(step_micros / rollRates[rollRates_cursor]);
  //rollRate_micros *= 32;

  rollRate_micros = round(((float)step_micros / rollRates[rollRates_cursor]) * 32);// sûrement plus lent mais plus précis
}


void update_LFO_Rate_micros() {
  if (bank[currentBank].pattern[currentPattern].lfo_enable[0]) {
    LFO[0].rate_micros = get_LFO_Rate_micros(0);
    LFO1_timer.setInterval(LFO[0].rate_micros, LFO_1_tick);
  }

  if (bank[currentBank].pattern[currentPattern].lfo_enable[1]) {
    LFO[1].rate_micros = get_LFO_Rate_micros(1);
    LFO2_timer.setInterval(LFO[1].rate_micros, LFO_2_tick);
  }
}


void get_Bpm() {
  bpm = round(3750000.0 / step_micros);
}


void manage_Reset_Things() {
  if ((global_Step_16PPQN >= get_pattern_Length(16)) || (digitalRead(resetInPin) == LOW)) {
    global_Step_16PPQN = (PLAYING) ? get_firstStep(16) : get_pattern_Length(16) - 1; // voir si plutôt mettre 0
    global_Step_4PPQN = (PLAYING) ? get_firstStep(4) : get_pattern_Length(4) - 1; // à vérif (quand PLAYING == 0 et qu'on lance PLAY via seq ext. type Ableton, si step 0 est joué)

    if (bank[currentBank].pattern[currentPattern].tracksRetrig == 1) {
      for (byte track = 0; track < TRACKS; ++track) {
        track_Step_16PPQN[track] = global_Step_16PPQN;
      }
    }

    global_Step_Counter16 = 0; //global_Step_Counter16 = bank[currentBank].pattern[currentPattern].firstStep ??
    LFO[0].counter = 0;
    LFO[1].counter = 0;
    if (songMode_Activated && digitalRead(resetInPin) == LOW)
      currentPattern = bank[currentBank].songPatterns[0];
  }

  if (global_Step_Counter16 >= 16) {
    global_Step_Counter16 = 0; // firstStepDisplay ?
  }

  if (arpCounter >= arpSteps)
    arpCounter = 0;

  if (screenConfig == 5 && recMode && (track_Step_16PPQN[selected_Track] == fillDataRecording_startStep)) {
    recMode = 0;
    fillDataRecording_startStep = 0;
  }
}
