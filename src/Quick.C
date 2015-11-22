ROOTTreeFileJune f;
WaveformAnalyzer w;

Quick() {
  //  if (!f.Open(0)) {cout << "File not found" << endl; return;}
  if (!f.Open("Files/BiSource/run00020.root")){cout << "File not found" << endl; return;}

}

Do(int ev) {
  int thresh = 15;
  w.SetTrapPars(250,250,300);
  f.GetEvent(ev);
  w.MakeTrap(f.NI_event.length, f.NI_event.wave);//,rise,risetop);
  w.Plot();
  vector<trigger_t> triglist;
  w.GetTriggers(thresh, triglist);
  w.PlotTrap();
  for (int cnt=0;cnt<triglist.size();cnt++) {
    cout << "Found triggers at " << triglist[cnt].TrapT << " with E " << triglist[cnt].TrapE <<  endl;
    cout << "Ave " << triglist[cnt].AveTrapE << endl;
    cout << "Mid " << triglist[cnt].MidTrapE << endl; 
  }
  //  w.CheckBaseline();
}

DoFit(int ev) {
  f.GetEvent(ev);
  w.MakeTrap(f.NI_event.length, f.NI_event.wave);//,rise,risetop);
  vector<Double_t> E;
  vector<Double_t> t;
  w.FitWave(50, E, t);
  w.PlotTrap();
  for (int cnt=0;cnt<t.size();cnt++)
    cout << "Found triggers at " << t[cnt] << " with E " << E[cnt] <<  endl;

}

Test(int file) {
  TH1D* timehist = (TH1D*)gROOT->FindObject("th");
  if (timehist !=0) delete timehist;
  timehist = 0;
  timehist = new TH1D("th","th",300,0,300);
  TrigTreeFile tf;
  tf.Open(file);
  double lasttime;
  tf.GetEvent(0);
  lasttime = tf.Trig_event.t;
  for (int i=1;i<tf.GetNumEvents();i++) {
    tf.GetEvent(i);
    timehist->Fill(tf.Trig_event.t - lasttime);
    lasttime = tf.Trig_event.t;
  }
  timehist->Draw();
  tf.Close();
}
