ROOTTreeFile f;
WaveformAnalyzer w;

Quick() {
  if (!f.Open(0)) {cout << "File not found" << endl; return;}
}

Do(int ev, int ch) {
  int thresh = 200;
  f.GetEvent(ev);
  w.MakeTrap(f.NI_event.ch[ch].wavelen, f.NI_event.ch[ch].wave);//,rise,risetop);
  w.Plot();
  vector<Double_t> E;
  vector<Double_t> t;
  w.GetTriggers(thresh, E, t);
  w.PlotTrap();
  for (int cnt=0;cnt<t.size();cnt++)
    cout << "Found triggers at " << t[cnt] << endl;
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
