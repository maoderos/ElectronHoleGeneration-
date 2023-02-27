#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4String.hh"
#include "G4UnitsTable.hh"
#include "G4Step.hh"
#include "G4AnalysisManager.hh"
#include "EventAction.hh"
#include "G4SteppingManager.hh"
#include "G4RunManager.hh"
#include <sstream>
#include <iostream>

using namespace std;

SteppingAction::SteppingAction(EventAction* _evt, DetectorConstruction* _det, PrimaryGeneratorAction* _primary): G4UserSteppingAction(), evt(_evt), det(_det), primary(_primary)
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  G4double eDep = step->GetTotalEnergyDeposit();
  if (eDep <= 0) return;
  G4double nielEnergyDep = step->GetNonIonizingEnergyDeposit();
  G4double ionizingEnergyDep = eDep - nielEnergyDep;
  G4String materialName = det->GetMaterialName();
  G4double targetThickness = det->GetSensitiveThickness()/cm;
  G4double materialDensity = det->GetSensitiveMaterial()->GetDensity()/(g/cm3);
  // Enable for validation purposes
  /*
  G4double stepLength = step->GetStepLength();
  G4cout << "STEP LENGTH: " << stepLength/nm << " nm" << G4endl; 
  */

  evt->AddEdepTotal(eDep);
  evt->AddEdepIonizing(ionizingEnergyDep);
  evt->AddEdepNiel(nielEnergyDep);

  if (step->GetTrack()->GetVolume()->GetName() == "Sensitive") {

    if (step->GetTrack()->GetParentID() == 0) {
       evt->AddEdepTotalPrimary(eDep/materialDensity/targetThickness);
       //evt->AddEdepNiel(nielEnergyDep/materialDensity/targetThickness);
       //evt->AddEdepIonizing(ionizingEnergyDep/materialDensity/targetThickness);
    }
  }
}
