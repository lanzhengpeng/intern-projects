#include <uf.h>
#include <uf_ui.h>
#include <uf_exit.h>
#include <string.h>
#include<uf_part.h>
#include <stdio.h>
#include<uf_undo.h>
#include <NXOpen/Session.hxx>
#include <NXOpen/PartCollection.hxx>
#include <NXOpen/Part.hxx>
#include <NXOpen/DexManager.hxx>
#include <NXOpen/Step203Importer.hxx>
#include <string>
#include <NXOpen/LayoutCollection.hxx> // 布局集合类，包含 FindObject 方法
#include <NXOpen/Layout.hxx>         // Layout 类定义
#include <NXOpen/NXObject.hxx>       // NXObject 基类，FindObject 返回类型
#include <NXOpen/ModelingView.hxx>
#include <NXOpen/ModelingViewCollection.hxx>
#include <fstream>
#include<uf_assem.h>
#include <thread>
#include <chrono>

using namespace std;
// 从完整文件路径中提取文件名（去掉扩展名），结果存入 part_name，bufsize 为缓冲区大小
void extractFileNameNoExt(const char* InputFile, char* part_name, size_t bufsize) {
	// 找到路径中最后一个反斜杠 '\\' 出现的位置，指向该字符
	const char* lastSlash = strrchr(InputFile, '\\');

	// 如果找到反斜杠，fileName 指向它后面的第一个字符（文件名起始处）
	// 否则，fileName 直接指向 InputFile（说明没有路径，只有文件名）
	const char* fileName = (lastSlash ? lastSlash + 1 : InputFile);

	// 将 fileName 拷贝到 part_name，最多拷贝 bufsize-1 个字符，防止溢出
	strncpy_s(part_name, bufsize, fileName, bufsize - 1);

	// 不需要手动加 '\0'，strncpy_s 会自动保证结尾

	// 在 part_name 中查找最后一个 '.' 出现的位置，通常是扩展名的起始点
	char* dot = strrchr(part_name, '.');

	// 如果找到 '.'，将其替换成 '\0'，实现截断字符串，去掉扩展名
	if (dot) {
		*dot = '\0';
	}
}
//改保存路径
void getOutputPartPathFromStep(const char* stepPath, char* prtPath, size_t bufsize) {
	strncpy(prtPath, stepPath, bufsize - 1);
	prtPath[bufsize - 1] = '\0';
	char* dot = strrchr(prtPath, '.');
	if (dot) {
		strcpy(dot, ".prt");  // 修改扩展名为 .prt
	}
	else {
		// 没有扩展名，直接追加
		strncat(prtPath, ".prt", bufsize - strlen(prtPath) - 1);
	}
}
//step->prt
void stepToPrt(NXOpen::Session* theSession, const char* InputFile, const char* part_fspec, const char* SettingFile) {
	theSession->Parts()->CloseAll(NXOpen::BasePart::CloseModifiedCloseModified, nullptr);
	// 创建一个 STEP203 文件导入器
	NXOpen::Step203Importer* step203Importer;
	step203Importer = theSession->DexManager()->CreateStep203Importer();
	//对导入器进行设置
	step203Importer->SetImportTo(NXOpen::Step203Importer::ImportToOptionNewPart);
	// 设置导入时简化几何体
	step203Importer->SetSimplifyGeometry(true);

	// 设置默认图层为1
	step203Importer->SetLayerDefault(1);

	// 设置导入的 STEP 文件路径
	step203Importer->SetInputFile(InputFile);

	// 设置导入后保存的 prt 文件路径
	step203Importer->SetOutputFile(part_fspec);

	// 设置导入时的配置文件路径(无这个文件）
	//step203Importer->SetSettingsFile(SettingFile);


	//设置导入类型
	auto objectTypes = step203Importer->ObjectTypes();

	// 导入曲线（如线、圆、样条）
	objectTypes->SetCurves(true);

	// 导入曲面（如修剪面、开放面）
	objectTypes->SetSurfaces(true);

	// 导入实体（封闭体建模数据）
	objectTypes->SetSolids(true);

	// 导入 PMI（尺寸、公差、注释等 3D 标注信息）
	objectTypes->SetPmiData(true);

	// 不打开文件，只导入（false）
	step203Importer->SetFileOpenFlag(false);

	// 执行导入，返回导入对象指针
	NXOpen::NXObject* nXObject;
	nXObject = step203Importer->Commit();
	// 销毁导入器，释放资源
	step203Importer->Destroy();

	/*NXOpen::Part* importedPart = dynamic_cast<NXOpen::Part*>(theSession->Parts()->Work());
	if (importedPart != nullptr)
	{
		importedPart->Save(NXOpen::BasePart::SaveComponentsTrue, NXOpen::BasePart::CloseAfterSaveFalse);
		UF_UI_write_listing_window("导入成功，保存工作部件\n");
	}
	else
	{
		UF_UI_write_listing_window("工作部件为空，无法保存\n");
	}*/

}
//image路径
void GetImageSaveDir(const char* prtPath, char* imageDir, size_t bufsize)
{
	if (prtPath == nullptr || imageDir == nullptr || bufsize == 0)
		return;

	// 找到最后一个路径分隔符的位置
	const char* lastSlash = strrchr(prtPath, '\\');
	size_t dirLen = lastSlash ? (lastSlash - prtPath) : 0;

	// 检查空间是否足够
	const char* suffix = "\\images";
	if (dirLen + strlen(suffix) + 1 >= bufsize)
		return;

	// 复制目录部分
	strncpy(imageDir, prtPath, dirLen);
	imageDir[dirLen] = '\0';

	// 拼接 \images
	strcat(imageDir, suffix);
}
//异步请求锁，锁住后面代码（5分钟）
bool WaitForFileReady(const char* filepath, int maxWaitMs = 1000 * 60 * 5, int intervalMs = 500)
{
	int waited = 0;
	size_t lastSize = 0;
	int stableCount = 0;

	while (waited < maxWaitMs)
	{
		std::ifstream file(filepath, std::ios::binary | std::ios::ate);
		if (file)
		{
			size_t size = file.tellg();
			if (size > 0)
			{
				if (size == lastSize)
				{
					stableCount++;
					if (stableCount >= 3)
					{
						// 文件连续稳定多次，认为安全
						return true;
					}
				}
				else
				{
					stableCount = 0; // 重置稳定计数器
				}
				lastSize = size;
			}
		}
		else
		{
			// 文件尚不存在，也等待
			stableCount = 0;
			lastSize = 0;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
		waited += intervalMs;
	}

	return false;  // 超时未成功
}
//开始截图
void screenShot(const char* InputFile, const char* ImagesSaveDir, const char* SettingFile)
{


	// 打开NX信息窗口
	//状态
	// 打开NX信息窗口
	//UF_UI_open_listing_window();
	// 用于存放错误或提示信息的缓冲区
	char msg[256];
	//获取part_name
	char part_name[MAX_FSPEC_BUFSIZE] = "";
	// 从输入路径 InputFile 中提取文件名(不含扩展名)，存入 part_name
	extractFileNameNoExt(InputFile, part_name, MAX_FSPEC_BUFSIZE);

	//prt创建路径
	char part_fspec[MAX_FSPEC_BUFSIZE];
	getOutputPartPathFromStep(InputFile, part_fspec, sizeof(part_fspec));

	//提示
	sprintf_s(msg, "prt路径是:");
	uc1601(msg, 0);
	uc1601(part_fspec, 0);

	// 获取当前 NX 会话指针
	NXOpen::Session* theSession = NXOpen::Session::GetSession();

	//转化为prt并且保存
	stepToPrt(theSession, InputFile, part_fspec, SettingFile);


	//等待文件写入完成
	if (!WaitForFileReady(part_fspec))
	{

		sprintf_s(msg, "等待PRT文件写入超时！\n");
		uc1601(msg, 0);
		return; // 或其他错误处理
	}

	//image保存路径
	char imageDir[MAX_FSPEC_BUFSIZE];
	GetImageSaveDir(part_fspec, imageDir, sizeof(imageDir));


	//提示
	sprintf_s(msg, "imageDir路径是:");
	uc1601(msg, 0);
	uc1601(imageDir, 0);



	tag_t partTag = NULL_TAG; // 用于存储打开的零件标签
	UF_PART_load_status_t load_status;
	// UF_PART_open 打开文件
	int status = UF_PART_open(part_fspec, &partTag, &load_status);
	//设置为工作部件
	status = UF_ASSEM_set_work_part(partTag);
	status = UF_PART_set_display_part(partTag);
	NXOpen::Part* workPart = dynamic_cast<NXOpen::Part*>(theSession->Parts()->Work());
	if (workPart != nullptr)
	{
		//importedPart->Save(NXOpen::BasePart::SaveComponentsTrue, NXOpen::BasePart::CloseAfterSaveFalse);
		sprintf(msg, "有工作部件");
		uc1601(msg, 0);
	}
	else
	{
		sprintf(msg, "无工作部件");
		uc1601(msg, 0);
		return;
	}

	// 在工作零件的布局集合中查找名称为 "L1" 的布局
	NXOpen::Layout* layout1 = dynamic_cast<NXOpen::Layout*>(workPart->Layouts()->FindObject("L1"));

	// 如果未找到指定布局，报错退出
	if (!layout1) {
		sprintf(msg, "Error: Layout L1 not found.");
		uc1601(msg, 0);
		return;
	}


	// 定义要截图的视图名称数组，共7个
	const char* view_names[] = { "Top", "Front", "Right", "Back", "Left", "Bottom", "Isometric" };

	// 视图数量
	const int num_views = 7;

	for (int i = 0; i < num_views; i++) {

		// 在工作零件的建模视图集合中找到指定视图对象
		NXOpen::ModelingView* modelingView = dynamic_cast<NXOpen::ModelingView*>(
			workPart->ModelingViews()->FindObject(view_names[i]));
		// 如果该视图不存在，打印错误并跳过该视图
		if (!modelingView) {
			sprintf(msg, "Error: View %s not found.", view_names[i]);
			uc1601(msg, 0);
			continue;
		}

		// 用找到的视图替换布局中的当前视图，实现切换视图
		layout1->ReplaceView(workPart->ModelingViews()->WorkView(), modelingView, true);




		// 准备保存图片文件名的缓冲区
		char filename[256];

		// 构造图片文件完整路径，格式为 "ImagesSaveDir\partName_viewName.jpg"
		sprintf_s(filename, "%s\\%s_%s.jpg", imageDir, part_name, view_names[i]);
		uc1601(filename, 0);
		// 调用 UF_DISP_create_image 创建当前视图的 JPG 截图
		int status = UF_DISP_create_image(
			filename,
			UF_DISP_JPEG,
			UF_DISP_ORIGINAL
		);

		// 如果截图失败，获取错误信息并提示
		if (status != 0) {
			char error_msg[133];
			UF_get_fail_message(status, error_msg);
			sprintf(msg, "Failed to create image for %s view: %s\n", view_names[i], error_msg);
			uc1601(msg, 0);
		}
		// 截图成功，提示成功消息
		else {
			sprintf(msg, "Image created successfully: %s\n", filename);
			uc1601(msg, 0);
		}
	}



	//关闭part 开发阶段，方便调试
	// 参数说明：
	// part_tag - 要关闭的零件
	// 1 - 关闭该零件及其所有子装配
	// 1 - 强制关闭，即使零件被修改也关闭，不弹窗
	UF_PART_close(partTag, 1, 1);

	UF_UNDO_delete_all_marks();

}
#include <Windows.h>

HWND FindNXWindow()
{
	HWND hwnd = NULL;
	EnumWindows([](HWND hWnd, LPARAM lParam) -> BOOL {
		wchar_t title[256];
		GetWindowTextW(hWnd, title, 256);

		if (wcsstr(title, L"NX 10") != nullptr) {
			*(HWND*)lParam = hWnd;
			return FALSE;  // 找到就终止
		}
		return TRUE;
		}, (LPARAM)&hwnd);

	return hwnd;
}
//激活窗口
void BringNXToFront()
{
	HWND hwnd = FindNXWindow();
	if (hwnd != NULL) {
		SetForegroundWindow(hwnd);      // 激活窗口
		ShowWindow(hwnd, SW_RESTORE);   // 还原窗口（从最小化恢复）
	}
}
//最小化窗口
void MinimizeNXWindow()
{
	HWND hwnd = FindNXWindow();
	if (hwnd != NULL)
	{
		DWORD dwForeThread = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
		DWORD dwAppThread = GetCurrentThreadId();

		// 降低线程间的输入阻隔（必要时提升权限）
		AttachThreadInput(dwAppThread, dwForeThread, TRUE);

		ShowWindow(hwnd, SW_MINIMIZE);  // 最小化
		SetForegroundWindow(GetDesktopWindow());  // 把焦点切回桌面防止打扰

		AttachThreadInput(dwAppThread, dwForeThread, FALSE);
	}
}


extern "C" DllExport void CreateImages(const char* InputFile, const char* ImagesSaveDir, const char* SettingFile)
{
	BringNXToFront();              // 保证窗口激活
	Sleep(500);                    // 稍微等一下，避免太快执行失败
	UF_initialize();  // 初始化UF环境
	screenShot(InputFile, ImagesSaveDir, SettingFile);
	UF_terminate();   // 终止UF环境
	MinimizeNXWindow();
}


/// 插件主函数，在 NX 中加载时执行
extern "C" DllExport void ufusr(char* param, int* return_code, int rlen)
{//""D:\\NX\\CreateImagesDllNX10\\D671-L1.stpD:\\NX\\00010008.step
	const char* InputFile = "D:\\NX\\CreateImagesDllNX10\\D671-L1.stp";
	const char* ImagesSaveDir = "D:\\NX\\CreateImagesDllNX10\\images";
	const char* SettingFile = "D:\\Siemens\\NX10\\STEP203UG\\ugstep203.def";
	// 打开NX信息窗口
	UF_initialize();  // 初始化UF环境
	CreateImages(InputFile, ImagesSaveDir, SettingFile);
	UF_terminate();   // 终止UF环境
}

/// 插件的卸载策略（NX调用）
extern "C" DllExport int ufusr_ask_unload()
{
	// UF_UNLOAD_IMMEDIATELY 表示插件执行完就自动卸载
	return UF_UNLOAD_IMMEDIATELY;
}
