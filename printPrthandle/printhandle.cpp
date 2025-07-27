#include <uf.h>
#include <uf_ui.h>
#include <uf_exit.h>
#include <string.h>
#include<uf_part.h>
#include <stdio.h>
#include<uf_assem.h>
#include<uf_obj.h>
#include <uf_modl.h>
/// 插件主函数，在 NX 中加载时执行
extern "C" DllExport void ufusr(char* param, int* return_code, int rlen)
{
	// 打开NX信息窗口
	UF_initialize();  // 初始化UF环境
	// 打开NX信息窗口
	UF_UI_open_listing_window();
	// 获取当前工作部件
	tag_t work_part = UF_ASSEM_ask_work_part();
	if (work_part == NULL_TAG) {
		UF_UI_write_listing_window("Failed to get work part.\n");
		UF_terminate();
		return;
	}
	UF_UI_write_listing_window("工作部件获取成功\n");
	uf_list_p_t face_list = NULL;
	tag_t obj_tag = NULL_TAG;
	int count = 0;
	int face_count = 0;
	// 第一次调用，obj_tag传NULL_TAG，开始遍历
	int status = UF_OBJ_cycle_objs_in_part(work_part, UF_solid_type, &obj_tag);

	char msg[256];
	while (status == 0 && obj_tag != NULL_TAG)
	{
		count++;
		sprintf_s(msg, "实体体 %d tag: 0x%X\n", count, (unsigned int)obj_tag);
		UF_UI_write_listing_window(msg);
		status = UF_MODL_ask_body_faces(obj_tag, &face_list);
		if (status != 0)
		{
			char err_msg[133];
			UF_get_fail_message(status, err_msg);
			UF_UI_write_listing_window("获取面列表失败: ");
			UF_UI_write_listing_window(err_msg);
			UF_UI_write_listing_window("\n");
		}
		else {
			UF_MODL_ask_list_count(face_list, &face_count);
			for (int i = 0; i < face_count; ++i)
			{
				tag_t face_tag;
				if (UF_MODL_ask_list_item(face_list, i, &face_tag) == 0)
				{
					char msg[256];
					sprintf_s(msg, "第 %d 个面 tag: 0x%X\n", i + 1, (unsigned int)face_tag);
					UF_UI_write_listing_window(msg);

					// 你还可以在这里获取面类型、法向量、几何信息等
					const char* handle_str = UF_TAG_ask_handle_of_tag(face_tag);
					sprintf_s(msg, "该面的handle字符串为: %s\n", handle_str);
					UF_UI_write_listing_window(msg);

				}
				else
				{
					UF_UI_write_listing_window("获取面 tag 失败\n");
				}
			}
			char msg[256];
			sprintf_s(msg, "实体体共有 %d 个面\n", face_count);
			UF_UI_write_listing_window(msg);
		}

		// 继续获取下一个实体体tag
		status = UF_OBJ_cycle_objs_in_part(work_part, UF_solid_type, &obj_tag);
	}

	sprintf_s(msg, "遍历结束，共找到 %d 个实体体\n", count);
	UF_UI_write_listing_window(msg);


	UF_terminate();   // 终止UF环境
}

/// 插件的卸载策略（NX调用）
extern "C" DllExport int ufusr_ask_unload()
{
	// UF_UNLOAD_IMMEDIATELY 表示插件执行完就自动卸载
	return UF_UNLOAD_IMMEDIATELY;
}
