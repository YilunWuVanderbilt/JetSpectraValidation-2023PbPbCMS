using namespace std;


TH1D *jetpt_ratio = new TH1D("jetpt_ratio","jetpt_ratio",100,0.,2.);
TH1D *jeteta_ratio = new TH1D("jeteta_ratio","jeteta_ratio",100,0.,2.);
TH1D *jetphi_ratio = new TH1D("jetphi_ratio","jetphi_ratio",100,0.,2.);


void HistRatio(TString input1 = "HiForestMiniAOD_RawFull.root", TString input2 = "HiForestMiniAOD_RawPrimFull.root", TString output = "jetSpectra_ratio.root"){

   std::cout << "running HistRatio()" << std::endl;
   std::cout << "inputFile1   = " << input1  << std::endl;
   std::cout << "inputFile2   = " << input2  << std::endl;
   std::cout << "outputFile  = " << output << std::endl;	


   TTree* treeJet = 0;
   TTree* treeJet2 = 0;
    
   TFile* file1 = 0;
    
   std::cout << "### opening file ###" << std::endl;
   file1 = TFile::Open(input1);
   file1->cd();

   treeJet = (TTree*) file1->Get("akCs4PFJetAnalyzer/t");
   Long64_t entries1 = treeJet->GetEntries();
   treeJet->SetBranchStatus("*",0);     // disable all branches
   treeJet->SetBranchStatus("jtpt",1);   // enable event information
   treeJet->SetBranchStatus("jteta",1);
   treeJet->SetBranchStatus("jtphi",1);
   treeJet->SetBranchStatus("nref",1);
   treeJet->SetBranchStatus("evt",1);

   const unsigned int maxJets = 10000;
    
   Float_t jtpt1[maxJets];
   Float_t jteta1[maxJets];
   Float_t jtphi1[maxJets];
   Int_t nref1;
   Int_t evt1;

   treeJet->SetBranchAddress("jtpt",&jtpt1);
   treeJet->SetBranchAddress("jteta",&jteta1);
   treeJet->SetBranchAddress("jtphi",&jtphi1);
   treeJet->SetBranchAddress("nref",&nref1);
   treeJet->SetBranchAddress("evt",&evt1);
   
   //file2
   TFile* file2 = 0;

   std::cout << "### opening file ###" << std::endl;
   file2 = TFile::Open(input2);
   file2->cd();
    
   treeJet2 = (TTree*) file2->Get("akCs4PFJetAnalyzer/t");
   Long64_t entries2 = treeJet->GetEntries();
   treeJet2->SetBranchStatus("*",0);     // disable all branches
   treeJet2->SetBranchStatus("jtpt",1);   // enable event information
   treeJet2->SetBranchStatus("jteta",1);
   treeJet2->SetBranchStatus("jtphi",1);                                                    
   treeJet2->SetBranchStatus("nref",1);
   treeJet2->SetBranchStatus("evt",1);

   Float_t jtpt2[maxJets];
   Float_t jteta2[maxJets];                                                        
   Float_t jtphi2[maxJets];                                                                
   Int_t nref2;                                                                             
   Int_t evt2;

   treeJet2->SetBranchAddress("jtpt",&jtpt2);                                              
   treeJet2->SetBranchAddress("jteta",&jteta2);                                            
   treeJet2->SetBranchAddress("jtphi",&jtphi2);                                            
   treeJet2->SetBranchAddress("nref",&nref2);                                              
   treeJet2->SetBranchAddress("evt",&evt2);     
   //cout<<entries1<<endl;
   
   // loop through reco objects
   for (Long64_t j_entry = 0; j_entry < 30000; ++j_entry){
      cout<<j_entry<<endl;
      Int_t j_entry_matched = 0;
      treeJet->GetEntry(j_entry);
      //cout <<j_entry<<" "<<"#jets:"<<nref1<<" "<<"jetpt:"<<jtpt1[0]<<endl;
      treeJet2->GetEntry(j_entry_matched);
      //cout <<"Raw event No.:"<< evt1 <<" "<<"RawPrim event No."<< evt2 <<endl;
      while (evt2 != evt1){
         j_entry_matched += 1;
         treeJet2->GetEntry(j_entry_matched);
         if(j_entry_matched>entries2){
            break;
         }
      }
      //Float_t jtpt1_compare = 0.;
      //Float_t jtpt2_compare = 0.;
      for(Int_t i_jet = 0; i_jet < nref1; i_jet++){
         //cout <<"Raw event:"<<jtpt1[i_jet]<<" "<<endl;
         if(jtpt1[i_jet]<60){continue;}
         for(Int_t j_jet = 0; j_jet < nref2; j_jet++){
            //cout <<"Raw event:"<<jtpt1[i_jet]<<" "<<"RawPrimi event:"<<jtpt2[j_jet]<<" "<<"Ratio"<<jtpt2[i_jet]/jtpt1[j_jet]<<endl;
            Float_t dr = sqrt(pow(jteta1[i_jet] - jteta2[j_jet], 2) + pow(jtphi1[i_jet] - jtphi2[j_jet], 2)); 
            Float_t dr_compare= 1.;
            Int_t count = 0;
            if(jtpt2[j_jet]<60){continue;}
            if (dr <0.1){
               count = count + 1;
               //jtpt1_compare = jtpt1[i_jet];
               //jtpt2_compare = jtpt2[j_jet];
               jetpt_ratio->Fill(jtpt2[j_jet]/jtpt1[i_jet]);
               //cout <<"Raw event:"<<jtpt1[i_jet]<<" "<<"RawPrim event:"<<jtpt2[j_jet]<<" "<<"Ratio"<<jtpt2[i_jet]/jtpt1[j_jet]<<endl;
               jeteta_ratio->Fill(jteta2[j_jet]/jteta1[i_jet]);
               jetphi_ratio->Fill(jtphi2[j_jet]/jtphi1[i_jet]);
               //dr_compare = dr;
            }
            
            //jetpt_ratio->Fill(jtpt2_compare/jtpt1_compare);
         }
      }
      //} //while loop
   }
		    
   TFile *fileOutput = TFile::Open(output,"recreate");

   jetpt_ratio->Write();
   jeteta_ratio->Write();
   jetphi_ratio->Write();

   fileOutput->Close();
    
}


