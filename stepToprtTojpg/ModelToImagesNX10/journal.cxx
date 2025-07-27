// NX 10.0.0.24
// Journal created by 11568 on Mon Jun 23 17:45:13 2025 中国标准时间
//
#include <uf_defs.h>
#include <NXOpen/NXException.hxx>
#include <NXOpen/Session.hxx>
#include <NXOpen/Builder.hxx>
#include <NXOpen/DexBuilder.hxx>
#include <NXOpen/DexManager.hxx>
#include <NXOpen/FileNew.hxx>
#include <NXOpen/NXObject.hxx>
#include <NXOpen/ObjectTypeSelector.hxx>
#include <NXOpen/Part.hxx>
#include <NXOpen/PartCollection.hxx>
#include <NXOpen/Session.hxx>
#include <NXOpen/Step203Importer.hxx>
using namespace NXOpen;

extern "C" DllExport int ufusr_ask_unload()
{
    return (int)NXOpen::Session::LibraryUnloadOptionImmediately;
}

extern "C" DllExport void ufusr(char *param, int *retCode, int paramLen)
{
    NXOpen::Session *theSession = NXOpen::Session::GetSession();
    // ----------------------------------------------
    //   Menu: 文件(F)->新建(N)...
    // ----------------------------------------------
    NXOpen::Session::UndoMarkId markId1;
    markId1 = theSession->SetUndoMark(NXOpen::Session::MarkVisibilityVisible, NXString("\345\274\200\345\247\213", NXString::UTF8));
    
    NXOpen::FileNew *fileNew1;
    fileNew1 = theSession->Parts()->FileNew();
    
    theSession->SetUndoMarkName(markId1, NXString("\346\226\260\345\273\272 \345\257\271\350\257\235\346\241\206", NXString::UTF8));
    
    NXOpen::Session::UndoMarkId markId2;
    markId2 = theSession->SetUndoMark(NXOpen::Session::MarkVisibilityInvisible, NXString("\346\226\260\345\273\272", NXString::UTF8));
    
    theSession->DeleteUndoMark(markId2, NULL);
    
    NXOpen::Session::UndoMarkId markId3;
    markId3 = theSession->SetUndoMark(NXOpen::Session::MarkVisibilityInvisible, NXString("\346\226\260\345\273\272", NXString::UTF8));
    
    fileNew1->SetTemplateFileName("model-plain-1-mm-template.prt");
    
    fileNew1->SetUseBlankTemplate(false);
    
    fileNew1->SetApplicationName("ModelTemplate");
    
    fileNew1->SetUnits(NXOpen::Part::UnitsMillimeters);
    
    fileNew1->SetRelationType("");
    
    fileNew1->SetUsesMasterModel("No");
    
    fileNew1->SetTemplateType(NXOpen::FileNewTemplateTypeItem);
    
    fileNew1->SetTemplatePresentationName(NXString("\346\250\241\345\236\213", NXString::UTF8));
    
    fileNew1->SetItemType("");
    
    fileNew1->SetSpecialization("");
    
    fileNew1->SetCanCreateAltrep(false);
    
    fileNew1->SetNewFileName("C:\\temp\\_model1.prt");
    
    fileNew1->SetMasterFileName("");
    
    fileNew1->SetMakeDisplayedPart(true);
    
    NXOpen::NXObject *nXObject1;
    nXObject1 = fileNew1->Commit();
    
    NXOpen::Part *workPart(theSession->Parts()->Work());
    NXOpen::Part *displayPart(theSession->Parts()->Display());
    theSession->DeleteUndoMark(markId3, NULL);
    
    fileNew1->Destroy();
    
    theSession->ApplicationSwitchImmediate("UG_APP_MODELING");
    
    NXOpen::Session::UndoMarkId markId4;
    markId4 = theSession->SetUndoMark(NXOpen::Session::MarkVisibilityVisible, "Enter Gateway");
    
    NXOpen::Session::UndoMarkId markId5;
    markId5 = theSession->SetUndoMark(NXOpen::Session::MarkVisibilityVisible, "Enter Modeling");
    
    // ----------------------------------------------
    //   Menu: 文件(F)->导入(M)->STEP203...
    // ----------------------------------------------
    NXOpen::Session::UndoMarkId markId6;
    markId6 = theSession->SetUndoMark(NXOpen::Session::MarkVisibilityVisible, NXString("\345\274\200\345\247\213", NXString::UTF8));
    
    NXOpen::Step203Importer *step203Importer1;
    step203Importer1 = theSession->DexManager()->CreateStep203Importer();
    
    step203Importer1->SetSimplifyGeometry(true);
    
    step203Importer1->SetLayerDefault(1);
    
    step203Importer1->SetInputFile("F:\\step_material_color\\TY10521.stp");
    
    step203Importer1->SetOutputFile("C:\\temp\\TY10521.prt");
    
    step203Importer1->SetSettingsFile("D:\\Program Files\\Siemens\\NX 10.0\\step203ug\\step203ug.def");
    
    step203Importer1->ObjectTypes()->SetCurves(true);
    
    step203Importer1->ObjectTypes()->SetSurfaces(true);
    
    step203Importer1->ObjectTypes()->SetSolids(true);
    
    step203Importer1->ObjectTypes()->SetPmiData(true);
    
    step203Importer1->SetInputFile("");
    
    step203Importer1->SetOutputFile("");
    
    theSession->SetUndoMarkName(markId6, NXString("\345\257\274\345\205\245\350\207\252 STEP203 \351\200\211\351\241\271 \345\257\271\350\257\235\346\241\206", NXString::UTF8));
    
    step203Importer1->SetInputFile("F:\\step_material_color\\TY10521.stp");
    
    step203Importer1->SetOutputFile("C:\\temp\\TY10521.prt");
    
    NXOpen::Session::UndoMarkId markId7;
    markId7 = theSession->SetUndoMark(NXOpen::Session::MarkVisibilityInvisible, NXString("\345\257\274\345\205\245\350\207\252 STEP203 \351\200\211\351\241\271", NXString::UTF8));
    
    theSession->DeleteUndoMark(markId7, NULL);
    
    NXOpen::Session::UndoMarkId markId8;
    markId8 = theSession->SetUndoMark(NXOpen::Session::MarkVisibilityInvisible, NXString("\345\257\274\345\205\245\350\207\252 STEP203 \351\200\211\351\241\271", NXString::UTF8));
    
    step203Importer1->SetFileOpenFlag(false);
    
    NXOpen::NXObject *nXObject2;
    nXObject2 = step203Importer1->Commit();
    
    theSession->DeleteUndoMark(markId8, NULL);
    
    theSession->SetUndoMarkName(markId6, NXString("\345\257\274\345\205\245\350\207\252 STEP203 \351\200\211\351\241\271", NXString::UTF8));
    
    step203Importer1->Destroy();
    
    // ----------------------------------------------
    //   Menu: 工具(T)->操作记录(J)->停止录制(S)
    // ----------------------------------------------
}
