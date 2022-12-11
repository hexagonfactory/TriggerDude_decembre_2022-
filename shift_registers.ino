void read_shift_registers() {
  last_pinValues = current_pinValues;

  digitalWrite(SR_Input_p_load, LOW);
  digitalWrite(SR_Input_p_load, HIGH);

  uint16_t byteValue = 0;

  // on lit les pads de pad1 jusqu'à pad 12 puis 4 btns -> ex : 1010011100000100 = 42756
  for (byte i = 0; i < DATA_WIDTH; ++i) {
    byteValue |= digitalRead(SR_Input_data) << ((DATA_WIDTH - 1) - i);
    digitalWrite(SR_Input_clock, HIGH);
    digitalWrite(SR_Input_clock, LOW);
  }

  current_pinValues = byteValue; // avec ex : current_pinValues = 42756

  for (byte i = 0; i < DATA_WIDTH; ++i) {
    if (i < TRACKS) {
      if  ((screenConfig == 1 && bank[currentBank].pattern[currentPattern].track_isGate[i] == 1) ||
           screenConfig == 6 ||
           screenConfig == 8  ||
           (chromaMode_KBoard && bank[currentBank].pattern[currentPattern].track_isGate[selected_Track]) ||
           chromaMode_Arp) { // DELETE / ROLL / ARP
        trigsIn_BitVal[i] = bitRead(current_pinValues, i);
      }

      else {
        trigsIn_BitVal[i] = bitRead(current_pinValues, i) & !bitRead(last_pinValues, i);
      }
    }

    else {
      btnsIn_BitVal[i - 12] = bitRead(current_pinValues, i);
    }
  }
}



bool pad_long_pressed(byte id) {
  return bitRead(last_pinValues, id) & bitRead(current_pinValues, id);
}



unsigned int get_decimalValue_Pads() {
  unsigned int result = 0;

  for (byte i = 0; i < TRACKS; ++i) {
    result += trigsIn_BitVal[i] << ((DATA_WIDTH - 1) - i); // essayer de créer un array trigsOut_BitVal[TRACKS] comme pour SEQ
  }

  /*for (byte i = 0; i < TRACKS; ++i) {
    if ((micros() - button_micros[i]) < trig_DURATION)
      result += trigsIn_BitVal[i] << ((DATA_WIDTH - 1) - i);
    else
      result += 0 << ((DATA_WIDTH - 1) - i);
    }*/

  return result;
}


unsigned int get_decimalValue_Roll() {
  unsigned int result = 0;

  if (delayBeforeTrig == false) {
    for (byte i = 0; i < TRACKS; ++i) {
      result += trigsIn_BitVal[i] << ((DATA_WIDTH - 1) - i);
      canPass1 = 0;
      canPass2 = 0;

      unsigned long t1 = micros();
      if ((t1 - button_micros[i]) >= trig_DURATION) {
        last_sr_roll_time1 = t1;
        result += 0 << ((DATA_WIDTH - 1) - i);
        delayBeforeTrig = true;
        canPass1 = 1;
        canPass2 = 0;
      }
    }
  }

  else {
    if (canPass1) {
      unsigned long t2 = micros();
      if ((t2 - last_sr_roll_time1) >= rollRate_micros) {
        last_sr_roll_time2 = t2;
        for (byte i = 0; i < TRACKS; ++i) {
          result += trigsIn_BitVal[i] << ((DATA_WIDTH - 1) - i);
        }
        canPass1 = 0;
        canPass2 = 1;
      }
    }

    if (canPass2) {
      unsigned long t3 = micros();
      if ((t3 - last_sr_roll_time2) >= trig_DURATION) {
        last_sr_roll_time1 = t3;
        for (byte i = 0; i < TRACKS; ++i) {
          result += 0 << ((DATA_WIDTH - 1) - i);
        }
        canPass1 = 1;
        canPass2 = 0;
      }
    }
  }

  return result;
}


unsigned int get_decimalValue_KBoard() {
  unsigned int result = 0;

  for (byte i = 0; i < TRACKS; ++i) {
    if (i == selected_Track)
      result += padPushed[selected_Track] << ((DATA_WIDTH - 1) - i);

    else
      result += 0 << ((DATA_WIDTH - 1) - i);
  }

  return result;
}


unsigned int get_decimalValue_Arp() {
  unsigned int result = 0;

  for (byte j = 0; j < DATA_WIDTH; ++j) {
    if (j < TRACKS) {
      if ((micros() - button_micros[j]) < trig_DURATION)
        result += trigsIn_BitVal[j] << ((DATA_WIDTH - 1) - j);
      else
        result += 0 << ((DATA_WIDTH - 1) - j);
    }

    else
      result += 0 << ((DATA_WIDTH - 1) - j);
  }

  return result;
}



unsigned int decimalValue_TOTAL() {
  unsigned int result = 0;

  if (PLAYING) {
    if (micros() - clock_time_16PPQN < trig_DURATION)
      result = get_decimalValue_Seq();
    else
      result = get_decimalValue_Seq2();
  }

  switch (screenConfig) {
    case 1:
      result |= get_decimalValue_Pads();
      break;

    case 8 :
      result |= get_decimalValue_Roll();
      break;

    case 33 :
      if (chromaMode_KBoard)
        result |= get_decimalValue_KBoard();
      else if (chromaMode_Arp)
        result |= get_decimalValue_Arp();
      break;
  }

  return result;
}


void shiftOutTrigs(unsigned long value) {
  digitalWrite(SR_Output_latch, LOW);
  shiftOut(SR_Output_data, SR_Output_clock, LSBFIRST, lowByte(value));
  shiftOut(SR_Output_data, SR_Output_clock, LSBFIRST, highByte(value));
  digitalWrite(SR_Output_latch, HIGH);
}


/*void make595Play_Trigs_from_Sequence(unsigned long decimVal_curStep, unsigned long decimVal_dur_curStep, unsigned long duration) {
  if (update_shiftRegisters) {
    shiftOutTrigs(decimVal_curStep);

    if ((micros() - duration) >= trig_DURATION) {
      update_shiftRegisters = false;
      shiftOutTrigs(decimVal_dur_curStep);
    }
  }
  }*/

void make595Play_Trigs(unsigned long decimVal_total) {
  //if (update_shiftRegisters) {
  shiftOutTrigs(decimVal_total);
  //}
}



/*void make595Play_Trigs_from_ChromaMode(unsigned long x, unsigned long butMicro) {
  if (timeFlag_pads) {
    shiftOutTrigs(x);

    if ((micros() - butMicro) >= trig_DURATION) {
      timeFlag_pads = false;
      shiftOutTrigs(0);
    }
  }
  }


  void make595Play_Trigs_from_ChromaArp(unsigned long x, unsigned long butMicro) {
  if (timeFlag_arp) {
    shiftOutTrigs(x);

    if ((micros() - butMicro) >= trig_DURATION ) {
      shiftOutTrigs(0);
      timeFlag_arp = false;
    }
  }
  }


  void make595Play_Trigs_from_Roll(unsigned long x, unsigned long butMicro) {
  if (timeFlag_pads_roll) {
    //unsigned int trigDuration = (rollRate_micros <= trig_DURATION) ? 2000 : trig_DURATION;
    if (delayBeforeTrig == false) {
      shiftOutTrigs(x);
      canPass1 = 0;
      canPass2 = 0;


      unsigned long t1 = micros();
      if ((t1 - butMicro) >= trig_DURATION) {
        last_sr_roll_time1 = t1;
        shiftOutTrigs(0);
        delayBeforeTrig = true;
        canPass1 = 1;
        canPass2 = 0;
      }
    }

    else {
      if (canPass1) {
        unsigned long t2 = micros();
        if ((t2 - last_sr_roll_time1) >= rollRate_micros) {
          last_sr_roll_time2 = t2;
          shiftOutTrigs(x);
          canPass1 = 0;
          canPass2 = 1;
        }
      }

      if (canPass2) {
        unsigned long t3 = micros();
        if ((t3 - last_sr_roll_time2) >= trig_DURATION) {
          last_sr_roll_time1 = t3;
          shiftOutTrigs(0);
          canPass1 = 1;
          canPass2 = 0;
          timeFlag_pads_roll = false;
        }
      }

    }
  }
  }*/



/*
  //Generate LFO waves
  //SINE :
  // Example program
  #include <iostream>
  #include <string>
  #include <math.h>

  int sine[256];
  int s;

  int main() {

  for (int i = 0; i < 256; i++ ) {

    float rads = 3.14159265359/180;
    s = (float)(32767+32767*sin(1.40625*i*rads));

    //s = 32768 + 32768 * sin( i * (3.14159265359 / 180.0));

    sine[i] = s;
  }


  for (const auto& e : sine) {
    std::cout << e << ", ";
  }
  }


  // RAMP UP :
  #include <iostream>
  #include <string>
  #include <math.h>

  int rampUp[256];
  int s;

  int main() {



  for (int i = 0; i < 256; i++)  //256
  {
    rampUp[i] = i*256;
  }




  for (const auto& e : rampUp) {
    std::cout << e << ", ";
  }
  }

*/
