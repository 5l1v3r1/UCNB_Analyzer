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
