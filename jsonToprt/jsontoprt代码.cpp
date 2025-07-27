#include <uf.h>
#include <cstdio>
#include <iostream>
#include <cstring>  // 要包含这个头文件，用来调用 strcpy 或 strncpy
#include <uf_part.h>
#include <json.hpp>
#include <fstream>        // std::ifstream，文件输入流
#include <uf_sket.h>
using json = nlohmann::json;
double pi = std::acos(-1);
#include <Eigen/Dense>
#include<uf_obj.h>
#include <vector>
#include <uf_ui.h>
#include <uf_modl.h>
//全局MAP映射，草图名->R变换矩阵
std::map<std::string, Eigen::Matrix3d> sketchRMap;
//全局MAP映射，草图名->T平移矩阵
std::map<std::string, Eigen::Vector3d> sketchTMap;
//全局MAP映射，草图名->line/arc
std::map<std::string, std::vector<tag_t>> lineTagsMap;
std::map<std::string, std::vector<tag_t>> arcTagsMap;
#include<uf.h>
#include<uf_sket.h>
//错误调试
int UF_get_fail_message(
	int error_code,         // 错误码
	char error_message[]    // 存储错误信息的字符串数组
);

//字符串处理函数，提取文件名
const char* extract_file_name(const char* full_path) {
	const char* last_backslash = std::strrchr(full_path, '\\');
	const char* last_slash = std::strrchr(full_path, '/');
	const char* file_name = last_backslash > last_slash ? last_backslash : last_slash;
	return file_name ? file_name + 1 : full_path;
}


//初始化部件，生成一个新的 PRT 文件
int init(int argc, char* argv[])
{
	if (UF_initialize() != 0) {
		printf("UG 初始化失败\n");
		return -1;
	}
	printf("UG 初始化成功\n");

	if (argc < 3) {
		std::cerr << "错误：请传入两个参数！" << std::endl;
		std::cerr << "用法示例：程序名 输入JSON文件路径 输出PRT文件路径" << std::endl;
		UF_terminate();
		return -1;
	}

	const char* json_input_path = argv[1]; // 第一个参数：输入 JSON 文件路径
	const char* prt_output_path = argv[2]; // 第二个参数：输出 PRT 文件路径

	std::cout << "输入 JSON 文件路径: " << json_input_path << std::endl;
	std::cout << "输出 PRT 文件路径: " << prt_output_path << std::endl;

	//对象（比如部件、特征、曲线等）的句柄或者标识符。
	tag_t part_tag;
	// 1: millimeters, 2: inches
	int units = 1;
	char part_name[MAX_FSPEC_BUFSIZE];
	// 获取文件名
	const char* fname = extract_file_name(argv[2]);
	// 复制文件名到 part_name
	strncpy_s(part_name, fname, MAX_FSPEC_BUFSIZE - 1);
	int status = UF_PART_new(part_name, units, &part_tag);
	// 检查是否创建成功
	if (status != 0)
	{
		printf("创建新prt文件失败");
		UF_terminate();  // 结束资源
		return -1;
	}
	//创建prt文件，成功进入下一步 暂时在内存里面
	printf("创建新prt文件成功\n");
	UF_PART_set_display_part(part_tag); // 显示
	return 0;
}


//读取 JSON 文件
json read_sketch_json(const char* file_path)
{
	std::ifstream input_file(file_path);
	if (!input_file.is_open()) {
		std::cerr << "无法打开文件: " << file_path << std::endl;
		return json{};  // 返回空 json 对象
	}
	json sketch_data;
	input_file >> sketch_data;
	return sketch_data;
}


//创建草图
tag_t create_sketch(const json& sketch_data, int index)
{
	Eigen::Matrix3d R;
	// 草图的名称动态拼接
	char name[UF_OBJ_NAME_BUFSIZE];
	snprintf(name, UF_OBJ_NAME_BUFSIZE, "MYSKETCH_%d", index);
	printf("Creating sketch with name: %s\n", name);


	// 读取草图定义数据（原来 sketch_data 是一个数组，现在已假设传入的是数组中的单个元素）
	auto first_item = sketch_data;

	// 获取坐标系数据
	auto csys_data = first_item["coordinate_system"];

	// 从JSON中提取欧拉角（Z-Y-X顺序）
	double euler_angles[3] = {
		csys_data["Euler Angles"][0],
		csys_data["Euler Angles"][1],
		csys_data["Euler Angles"][2]
	};


	// 从JSON中提取平移向量
	double translation[3] = {
		csys_data["Translation Vector"][0],
		csys_data["Translation Vector"][1],
		csys_data["Translation Vector"][2]
	};

	// 将欧拉角赋值给alpha、beta、gamma（单位为角度）
	double alpha = euler_angles[0]; // 绕Z轴旋转（Yaw）
	double beta = euler_angles[1];  // 绕Y轴旋转（Pitch）
	double gamma = euler_angles[2]; // 绕X轴旋转（Roll）

	// 将角度转换为弧度
	double alpha_rad = alpha * pi / 180.0;
	double beta_rad = beta * pi / 180.0;
	double gamma_rad = gamma * pi / 180.0;

	// 计算欧拉角对应的三角函数值（旋转矩阵将由它们构成）
	double cos_a = cos(alpha_rad); // cos(alpha)
	double sin_a = sin(alpha_rad); // sin(alpha)
	double cos_b = cos(beta_rad);  // cos(beta)
	double sin_b = sin(beta_rad);  // sin(beta)
	double cos_g = cos(gamma_rad); // cos(gamma)
	double sin_g = sin(gamma_rad); // sin(gamma)

	// 构造旋转矩阵 R（Z-Y-X 欧拉角顺序）
	R(0, 0) = cos_a * cos_b;
	R(0, 1) = cos_a * sin_b * sin_g - sin_a * cos_g;
	R(0, 2) = cos_a * sin_b * cos_g + sin_a * sin_g;

	R(1, 0) = sin_a * cos_b;
	R(1, 1) = sin_a * sin_b * sin_g + cos_a * cos_g;
	R(1, 2) = sin_a * sin_b * cos_g - cos_a * sin_g;

	R(2, 0) = -sin_b;
	R(2, 1) = cos_b * sin_g;
	R(2, 2) = cos_b * cos_g;

	//存储R矩阵
	sketchRMap[name] = R;

	//存储T矩阵
	sketchTMap[name][0] = translation[0];
	sketchTMap[name][1] = translation[1];
	sketchTMap[name][2] = translation[2];


	// --- （调试用）可以在这里弹出提示，显示欧拉角和坐标信息
	char msg[128];
	sprintf_s(msg, sizeof(msg), "%f%f%f", euler_angles[0], euler_angles[1], euler_angles[2]);
	uc1601(msg, 1);
	printf("Translation Vector: %f %f %f\n", translation[0], translation[1], translation[2]);


	// 定义选项参数（通常设置为2表示使用用户提供的矩阵）
	int option = 2;

	// 构造变换矩阵（3x3旋转 + 1x3平移）作为一个3x4矩阵展开成一维数组
	double matrix[12] = {
	R(0,0), R(0,1), R(0,2),   // 第一行：R 的第一行
	R(1,0), R(1,1), R(1,2),   // 第二行：R 的第二行
	R(2,0), R(2,1), R(2,2),   // 第三行：R 的第三行
	translation[0], translation[1], translation[2]  // 第四行：T 向量
	};



	// 草图方向定义（1 表示沿正方向）
	int plane_dir = 1;

	// 用于接收创建的草图对象的tag
	tag_t sketch_tag = NULL_TAG;

	// 初始化草图环境（必要步骤，否则可能导致创建失败）
	UF_SKET_initialize_sketch(name, &sketch_tag);

	// 创建草图对象，使用指定的名称、矩阵等
	UF_SKET_create_sketch(name, option, matrix, NULL, NULL, plane_dir, &sketch_tag);

	// 打印草图的旋转和平移矩阵
	for (const auto& [name, R] : sketchRMap) {
		std::cout << "Sketch name: " << name << std::endl;

		std::cout << "Matrix R (Rotation):" << std::endl;
		std::cout << R << std::endl;

		// 检查是否存在对应的平移向量
		if (sketchTMap.find(name) != sketchTMap.end()) {
			Eigen::Vector3d T = sketchTMap[name];
			std::cout << "Vector T (Translation):" << std::endl;
			std::cout << T.transpose() << std::endl;  // 横着打印
		}
		else {
			std::cout << "Translation vector not found." << std::endl;
		}

		std::cout << std::endl;
	}


	// 返回新建的草图tag（用于后续操作）
	return sketch_tag;
}
std::vector<tag_t> create_sketches(const json& sketch_data_array)
{
	size_t index = 0;
	std::vector<tag_t> sketch_tags; // 用于存储创建的草图标签
	for (const auto& item : sketch_data_array) {
		tag_t sketch_tag = create_sketch(item, index);
		index++;
		if (sketch_tag != NULL_TAG) {
			sketch_tags.push_back(sketch_tag); // 将成功创建的草图标签添加到向量中
		}
		else {
			std::cerr << "创建草图失败" << std::endl;
		}
	}
	return sketch_tags;
}


// 根据 sketch_data 中的信息，向 sketch_tag 指定的草图中添加线段和圆弧几何体
void create_sketch_geometry(tag_t sketch_tag, const json& sketch_data)
{
	char sketch_name[UF_OBJ_NAME_BUFSIZE];
	UF_OBJ_ask_name(sketch_tag, sketch_name);  // 获取草图名称
	//可删除：打印草图名称
	printf("Sketch name: %s\n", sketch_name);

	// 获取 JSON 数组中的元素（一个草图数据对象）
	auto first_item = sketch_data;

	// 获取该草图的几何信息（包含若干 line_ 和 arc_ 元素）
	auto sketch_geom = first_item["sketch"];

	// 初始化线段和圆弧的数量
	int line_count = 0;
	int arc_count = 0;
	printf("开始处理草图几何体...\n");

	// 遍历草图图元，统计线段与圆弧数量
	for (const auto& item : sketch_geom.items())
	{
		if (item.key().find("line_") != std::string::npos)
		{
			line_count++;  // 发现一条线段
		}
		if (item.key().find("arc_") != std::string::npos)
		{
			arc_count++;  // 发现一个圆弧
		}
	}

	// 为当前草图添加对应的 vector 到 map 中，并提前分配空间
	lineTagsMap[sketch_name] = std::vector<tag_t>(line_count);
	arcTagsMap[sketch_name] = std::vector<tag_t>(arc_count);


	// 可选：用于弹窗提示图元统计信息（可删除）
	char msg[64];
	sprintf_s(msg, sizeof(msg), "%d条直线 %d个圆弧", line_count, arc_count);
	uc1601(msg, 1);


	// 初始化索引
	int line_index = 0;
	int arc_index = 0;

	// 返回值变量，用于检测 UF 函数是否执行成功
	int info = 0;

	// 遍历所有几何图元，逐个处理
	for (const auto& item : sketch_geom.items())
	{
		// ==== 处理直线 ====
		if (item.key().find("line_") != std::string::npos)
		{
			// 读取起点和终点的 sketch 坐标（二维）
			double start[2] = { item.value()["start"][0], item.value()["start"][1] };
			double end[2] = { item.value()["end"][0], item.value()["end"][1] };

			// 将二维草图坐标转换为三维坐标（z=0）
			Eigen::Vector3d sketch_start(start[0], start[1], 0.0);
			Eigen::Vector3d sketch_end(end[0], end[1], 0.0);

			// 获取当前草图对应的旋转矩阵 R 和平移向量 T
			const Eigen::Matrix3d& R = sketchRMap[sketch_name];
			const Eigen::Vector3d& T = sketchTMap[sketch_name];

			// 执行逆变换：world_point = R⁻¹ * (sketch_point - T)
			Eigen::Vector3d real_start = R.inverse() * (sketch_start + T);
			Eigen::Vector3d real_end = R.inverse() * (sketch_end + T);


			// 转换为 UF 用的坐标数组
			double start_point[3] = { real_start.x(), real_start.y(), real_start.z() };
			double end_point[3] = { real_end.x(), real_end.y(), real_end.z() };

			// 创建起点和终点的 NX 点对象
			tag_t start_p;
			tag_t end_p;
			UF_CURVE_create_point(start_point, &start_p);
			UF_CURVE_create_point(end_point, &end_p);

			// 设置线段的两个端点作为限制
			UF_CURVE_limit_s limit[2];
			UF_CURVE_limit_p_t pLimit[2] = { &limit[0], &limit[1] };
			limit[0].limit_type = UF_CURVE_limit_to_entity;
			limit[0].limiting_obj = start_p;
			limit[1].limit_type = UF_CURVE_limit_to_entity;
			limit[1].limiting_obj = end_p;

			// 创建线段对象（通过两点）
			tag_t lineTag = NULL_TAG;
			UF_CURVE_create_line_point_point(start_p, end_p, pLimit, NULL_TAG, false, &lineTag);

			// 存入全局/外部 lineTags 容器
			lineTagsMap[sketch_name][line_index] = lineTag;
			line_index += 1;

			std::cout << "sketch_tag = " << sketch_tag << std::endl;

			// 终止以前的草图
			UF_SKET_terminate_sketch();

			// 初始化已有草图
			int err = UF_SKET_initialize_sketch(sketch_name, &sketch_tag);  // 激活已有草图

			// 将该线段添加到草图中
			int info = UF_SKET_add_objects(sketch_tag, 1, &lineTag);

			// 检查是否添加成功
			if (info != 0)
			{
				char error_message[256];
				UF_get_fail_message(info, error_message);
				printf("UF_SKET_add_objects 错误 %d: %s\n", info, error_message);

				char msg[32];
				sprintf_s(msg, sizeof(msg), "第%d条直线添加失败，返回值", line_index);
				uc1601(msg, 1);
			}
			else
			{
				char msg[32];
				sprintf_s(msg, sizeof(msg), "第%d条直线添加成功", line_index);
				uc1601(msg, 1);
			}
		}


		// ==== 处理圆弧 ====
		if (item.key().find("arc_") != std::string::npos)
		{
			// 获取圆弧的三点坐标（起点、中点、终点）
			double start[2] = { item.value()["start"][0], item.value()["start"][1] };
			double middle[2] = { item.value()["middle"][0], item.value()["middle"][1] };
			double end[2] = { item.value()["end"][0], item.value()["end"][1] };

			// 变换矩阵
			Eigen::Matrix3d R_inv = sketchRMap[sketch_name].inverse();
			Eigen::Vector3d T_inv = -R_inv * sketchTMap[sketch_name];

			// 坐标变换：将草图二维坐标转为三维(z=0)，然后变换到模型坐标系
			Eigen::Vector3d sketch_start(start[0], start[1], 0.0);
			Eigen::Vector3d real_start = R_inv * sketch_start + T_inv;
			double start_point[3] = { real_start.x(), real_start.y(), real_start.z() };

			Eigen::Vector3d sketch_middle(middle[0], middle[1], 0.0);
			Eigen::Vector3d real_middle = R_inv * sketch_middle + T_inv;
			double middle_point[3] = { real_middle.x(), real_middle.y(), real_middle.z() };

			Eigen::Vector3d sketch_end(end[0], end[1], 0.0);
			Eigen::Vector3d real_end = R_inv * sketch_end + T_inv;
			double end_point[3] = { real_end.x(), real_end.y(), real_end.z() };

			// 创建三点对象（用于画圆弧）
			tag_t start_p, middle_p, end_p;
			UF_CURVE_create_point(start_point, &start_p);
			UF_CURVE_create_point(middle_point, &middle_p);
			UF_CURVE_create_point(end_point, &end_p);

			// 设置限制（起点和终点）
			UF_CURVE_limit_s limit[2];
			UF_CURVE_limit_p_t pLimit[2] = { &limit[0], &limit[1] };
			limit[0].limit_type = UF_CURVE_limit_to_entity;
			limit[0].limiting_obj = start_p;
			limit[1].limit_type = UF_CURVE_limit_to_entity;
			limit[1].limiting_obj = end_p;

			// 通过三点创建圆弧
			tag_t arcTag = NULL_TAG;
			UF_CURVE_create_arc_3point(start_p, end_p, middle_p, pLimit, NULL_TAG, false, &arcTag);

			// 存入 arcTags 向量
			arcTagsMap[sketch_name][arc_index] = arcTag;
			arc_index += 1;

			// 将圆弧添加到草图
			info = UF_SKET_add_objects(sketch_tag, 1, &arcTag);

			// 失败则弹窗提示
			if (info != 0)
			{
				char msg[32];
				sprintf_s(msg, sizeof(msg), "第%d个圆弧添加失败", arc_index);
				uc1601(msg, 1);
			}
		}

	}

	// 所有图元添加完毕后，更新草图
	UF_SKET_update_sketch(sketch_tag);
}
void create_sketches_geometry(const std::vector<tag_t>& sketch_tags, const json& sketch_data_array)
{
	for (size_t i = 0; i < sketch_tags.size(); ++i)
	{
		tag_t sketch_tag = sketch_tags[i];
		const json& sketch_data = sketch_data_array[i];

		create_sketch_geometry(sketch_tag, sketch_data);
	}
}

// 添加约束
void add_sketch_constraints(tag_t sketch_tag, const json& sketch_data)
{

	// 获取草图数据中的一个草图
	auto first_item = sketch_data;

	// 解析出该草图的“constraints”约束字段
	auto sketch_constraints = first_item["constraints"];

	//终止以前的草图
	UF_SKET_terminate_sketch();
	// 初始化已有草图
	char sketch_name[UF_OBJ_NAME_BUFSIZE];
	UF_OBJ_ask_name(sketch_tag, sketch_name);  // 获取草图名称
	int err = UF_SKET_initialize_sketch(sketch_name, &sketch_tag);  // 激活已有草图

	// 添加“垂直”约束（Perpendicular）
	if (sketch_constraints.contains("Perpendicular") && sketch_constraints["Perpendicular"].is_array()) {

		for (auto& pair : sketch_constraints["Perpendicular"])
		{
			// 获取两个线段的名称，如 Line_1
			std::string line1 = pair[0].get<std::string>();
			std::string line2 = pair[1].get<std::string>();

			// 提取编号部分（用于获取正确的 tag）
			size_t split_pos1 = line1.find('_');
			size_t split_pos2 = line2.find('_');
			int line1_index = std::stoi(line1.substr(split_pos1 + 1));
			int line2_index = std::stoi(line2.substr(split_pos2 + 1));

			// 设置约束类型为垂直
			UF_SKET_con_type_t con_type = UF_SKET_perpendicular;
			UF_SKET_con_geom_s con_geoms[2];  // 两个几何体参与约束
			int   num_con_geoms = 2;
			tag_t geom_con_tag;

			// 设置几何体的 tag（假设 lineTags 中存储了草图元素的 tag）
			con_geoms[0].geom_tag = lineTagsMap[sketch_name][line1_index - 1];
			con_geoms[1].geom_tag = lineTagsMap[sketch_name][line2_index - 1];

			// 创建几何约束
			UF_SKET_create_geometric_constraint(sketch_tag, con_type, num_con_geoms, con_geoms, &geom_con_tag);
		}
	}
	else {
		printf("Perpendicular 字段不存在或不是数组，跳过该约束类型。\n");
	}


	// 添加“平行”约束（Parallel）
	if (sketch_constraints.contains("Parallel") && sketch_constraints["Parallel"].is_array()) {
		for (auto& pair : sketch_constraints["Parallel"]) {
			if (!pair.is_array() || pair.size() != 2) {
				printf("Parallel约束格式错误，跳过该项。\n");
				continue;
			}

			std::string line1 = pair[0].get<std::string>();
			std::string line2 = pair[1].get<std::string>();

			size_t split_pos1 = line1.find('_');
			size_t split_pos2 = line2.find('_');
			if (split_pos1 == std::string::npos || split_pos2 == std::string::npos) {
				printf("Parallel约束命名格式错误，跳过该项。\n");
				continue;
			}

			int line1_index = std::stoi(line1.substr(split_pos1 + 1));
			int line2_index = std::stoi(line2.substr(split_pos2 + 1));

			if (line1_index <= 0 || line2_index <= 0 ||
				line1_index > lineTagsMap[sketch_name].size() || line2_index > lineTagsMap[sketch_name].size()) {
				printf("Parallel约束索引越界，跳过该项。\n");
				continue;
			}

			UF_SKET_con_type_t con_type = UF_SKET_parallel;
			UF_SKET_con_geom_s con_geoms[2];
			int num_con_geoms = 2;
			tag_t geom_con_tag;

			con_geoms[0].geom_tag = lineTagsMap[sketch_name][line1_index - 1];
			con_geoms[1].geom_tag = lineTagsMap[sketch_name][line2_index - 1];

			UF_SKET_create_geometric_constraint(sketch_tag, con_type, num_con_geoms, con_geoms, &geom_con_tag);
		}
	}
	else {
		printf("未找到 Parallel 约束，或数据格式错误，已跳过。\n");
	}


	// 添加“水平”约束（Horizontal）
	if (sketch_constraints.contains("Horizontal") && sketch_constraints["Horizontal"].is_array()) {
		int num_of_horizontal_constraints = sketch_constraints["Horizontal"].size();
		for (int i = 0; i < num_of_horizontal_constraints; i++) {
			if (!sketch_constraints["Horizontal"][i].is_string()) {
				printf("第 %d 个 Horizontal 约束格式错误，跳过。\n", i);
				continue;
			}

			std::string line = sketch_constraints["Horizontal"][i].get<std::string>();
			size_t split_pos = line.find('_');
			if (split_pos == std::string::npos) {
				printf("第 %d 个 Horizontal 命名格式错误，跳过。\n", i);
				continue;
			}

			int line_index = std::stoi(line.substr(split_pos + 1));
			if (line_index <= 0 || line_index > lineTagsMap[sketch_name].size()) {
				printf("第 %d 个 Horizontal 索引越界，跳过。\n", i);
				continue;
			}

			UF_SKET_con_type_t con_type = UF_SKET_horizontal;
			UF_SKET_con_geom_s con_geoms[1];
			int num_con_geoms = 1;
			tag_t geom_con_tag;

			con_geoms[0].geom_tag = lineTagsMap[sketch_name][line_index - 1];

			UF_SKET_create_geometric_constraint(sketch_tag, con_type, num_con_geoms, con_geoms, &geom_con_tag);
		}
	}
	else {
		printf("未找到 Horizontal 约束或数据格式错误，已跳过。\n");
	}


	// 添加“相切”约束（Tangent）
	if (sketch_constraints.contains("Tangent") && sketch_constraints["Tangent"].is_array())
	{
		for (auto& pair : sketch_constraints["Tangent"])
		{
			std::string line1 = pair[0].get<std::string>();
			std::string arc1 = pair[1].get<std::string>();

			size_t split_pos1 = line1.find('_');
			size_t split_pos2 = arc1.find('_');
			int line1_index = std::stoi(line1.substr(split_pos1 + 1));
			int arc1_index = std::stoi(arc1.substr(split_pos2 + 1));

			UF_SKET_con_type_t con_type = UF_SKET_tangent;
			UF_SKET_con_geom_s con_geoms[2];
			int   num_con_geoms = 2;
			tag_t geom_con_tag;

			con_geoms[0].geom_tag = lineTagsMap[sketch_name][line1_index - 1];
			con_geoms[1].geom_tag = arcTagsMap[sketch_name][arc1_index - 1];

			UF_SKET_create_geometric_constraint(sketch_tag, con_type, num_con_geoms, con_geoms, &geom_con_tag);
		}
	}
	else {
		printf("未找到 Tangent 约束或数据格式错误，已跳过。\n");
	}


	// 添加“相等半径”约束（Equal），用于弧之间的相等半径约束
	if (sketch_constraints.contains("Equal") && sketch_constraints["Equal"].is_array())
	{
		for (auto& pair : sketch_constraints["Equal"])
		{
			int num_of_equal_arc = pair[0].size();  // 假设是一组 arc

			UF_SKET_con_type_t con_type = UF_SKET_equal_radius;
			UF_SKET_con_geom_s con_geoms[100];  // 假设最多 100 个弧
			int num_con_geoms = num_of_equal_arc;

			for (int i = 0; i < num_of_equal_arc; i++)
			{
				std::string arc = pair[i].get<std::string>();
				size_t split_pos = arc.find('_');
				int arc_index = std::stoi(arc.substr(split_pos + 1));
				con_geoms[i].geom_tag = arcTagsMap[sketch_name][arc_index - 1];
			}

			tag_t geom_con_tag;
			UF_SKET_create_geometric_constraint(sketch_tag, con_type, num_con_geoms, con_geoms, &geom_con_tag);
		}
	}
	else {
		printf("未找到 Equal 约束或数据格式错误，已跳过。\n");
	}


	// 添加“垂直”约束（Vertical）
	if (sketch_constraints.contains("Vertical") && sketch_constraints["Vertical"].is_array()) {
		int num_of_vertical_constraints = sketch_constraints["Vertical"].size();
		for (int i = 0; i < num_of_vertical_constraints; i++)
		{
			std::string line = sketch_constraints["Vertical"][i].get<std::string>();
			size_t split_pos = line.find('_');
			int line_index = std::stoi(line.substr(split_pos + 1));

			UF_SKET_con_type_t con_type = UF_SKET_vertical;
			UF_SKET_con_geom_s con_geoms[1];
			int num_con_geoms = 1;
			tag_t geom_con_tag;
			con_geoms[0].geom_tag = lineTagsMap[sketch_name][line_index - 1];

			UF_SKET_create_geometric_constraint(sketch_tag, con_type, num_con_geoms, con_geoms, &geom_con_tag);
		}
	}
	else {
		printf("未找到 Vertical 约束或数据格式错误，已跳过。\n");
	}


	//添加“重合约束”（Coincident）
	if (sketch_constraints.contains("Coincident") && sketch_constraints["Coincident"].is_array()) {
		printf("重合约束开始\n");
		for (auto& pair : sketch_constraints["Coincident"])
		{
			// 获取两个线段点的名称
			std::string point1 = pair[0].get<std::string>();// 例如 line_4.end
			std::string point2 = pair[1].get<std::string>();// 例如 line_5.start

			//提取线段编号与端点类型
			// line_4.end → 分成 line_4 和 end
			auto split_dot = [](const std::string& s) {
				size_t dot_pos = s.find('.');
				return std::make_pair(s.substr(0, dot_pos), s.substr(dot_pos + 1));
				};

			// C++17标准
			auto [lineA, posA] = split_dot(point1);  // line_4, end
			auto [lineB, posB] = split_dot(point2);  // line_5, start

			// 获取索引编号
			int idxA = std::stoi(lineA.substr(lineA.find('_') + 1)) - 1;
			int idxB = std::stoi(lineB.substr(lineB.find('_') + 1)) - 1;
			printf("idxA=%d idxB=%d\n", idxA, idxB);

			//调试
			printf("lineTags.size() = %zu\n", lineTagsMap[sketch_name].size());
			printf("idxA=%d, idxB=%d\n", idxA, idxB);

			if (idxA < 0 || idxA >= (int)lineTagsMap[sketch_name].size() || idxB < 0 || idxB >= (int)lineTagsMap[sketch_name].size()) {
				printf("索引越界，跳过该约束：idxA=%d, idxB=%d\n", idxA, idxB);
				continue;
			}

			// 获取线段 tag
			tag_t line_tagA = lineTagsMap[sketch_name][idxA];
			tag_t line_tagB = lineTagsMap[sketch_name][idxB];

			// 获取线段数据
			UF_CURVE_line_t line_dataA, line_dataB;
			UF_CURVE_ask_line_data(line_tagA, &line_dataA);
			UF_CURVE_ask_line_data(line_tagB, &line_dataB);

			// 根据 pos 决定使用起点还是终点
			double* coordA = (posA == "start") ? line_dataA.start_point : line_dataA.end_point;
			double* coordB = (posB == "start") ? line_dataB.start_point : line_dataB.end_point;

			// 创建点对象
			tag_t point_tagA, point_tagB;
			UF_CURVE_create_point(coordA, &point_tagA);
			UF_CURVE_create_point(coordB, &point_tagB);

			// 创建重合约束
			UF_SKET_con_geom_s con_geoms[2];
			con_geoms[0].geom_tag = point_tagA;  // 第一个点的tag
			con_geoms[1].geom_tag = point_tagB;  // 第二个点的tag
			tag_t constraint_tag;
			UF_SKET_create_geometric_constraint(sketch_tag, UF_SKET_coincident, 2, con_geoms, &constraint_tag);

		}
	}
	else {
		printf("Coincident 字段不存在或不是数组，跳过该约束类型。\n");
	}


}
void add_sketches_constraints(const std::vector<tag_t>& sketch_tags, const json& sketch_data_array) {
	// 确保草图数量和 JSON 数组长度匹配
	if (sketch_tags.size() != sketch_data_array.size())
	{
		std::cerr << "Error: sketch_tags size does not match sketch_data_array size!" << std::endl;
		return;
	}
	// 遍历每个草图和对应的 JSON 数据，调用已有约束添加函数
	for (size_t i = 0; i < sketch_tags.size(); ++i)
	{
		tag_t sketch_tag = sketch_tags[i];
		const json& sketch_data = sketch_data_array[i];

		add_sketch_constraints(sketch_tag, sketch_data);
	}
}

// 对草图的操作
void operation_sketch(tag_t sketch_tag, const json& sketch_data)
{
	//要操作的类型 默认新建
	UF_FEATURE_SIGN sign = UF_NULLSIGN;
	//终止以前的草图
	UF_SKET_terminate_sketch();
	// 初始化已有草图
	char sketch_name[UF_OBJ_NAME_BUFSIZE];
	UF_OBJ_ask_name(sketch_tag, sketch_name);  // 获取草图名称
	int err = UF_SKET_initialize_sketch(sketch_name, &sketch_tag);  // 激活已有草图
	// 从 JSON 中取出第一个拉伸参数对象
	auto first_item = sketch_data;

	// 获取“向拉伸”和“反向拉伸”的距离参数
	double extrude_towards = first_item["towards"];
	double extrude_opposite = first_item["opposite"];
	// 如果 opposite > 0，说明需要反向拉伸
	if (extrude_opposite > 0) {
		extrude_opposite = -extrude_opposite;  // 反向拉伸
	}

	// 获取操作类型"NewBody"：创建新实体。"Intersect"：交集。
	//"Join"：合并到现有实体。"Cut"：从现有实体中切割。
	auto op = first_item["operation"];

	if (op == "NewBody") {
		sign = UF_NULLSIGN;  // 默认值，表示新建实体
	}
	else if (op == "Join") {
		sign = UF_POSITIVE;  // 表示合并（加法）
	}
	else if (op == "Cut") {
		sign = UF_NEGATIVE;  // 表示切割（减法）
	}
	else if (op == "Intersect") {
		sign = UF_UNSIGNED;  // 表示交集
	}
	else {
		uc1601("不支持的布尔运算类型", 1);  // 自定义错误提示
	}


	// 创建几何体列表，用于存储要拉伸的草图线和圆弧
	uf_list_p_t LineList;
	UF_MODL_create_list(&LineList);

	// 统计总元素个数（线段 + 圆弧）
	int n = lineTagsMap[sketch_name].size() + arcTagsMap[sketch_name].size();

	// 将所有草图线添加进 UF 的列表
	for (int i = 0; i < lineTagsMap[sketch_name].size(); i++)
	{
		UF_MODL_put_list_item(LineList, lineTagsMap[sketch_name][i]);
	}

	// 将所有草图圆弧添加进 UF 的列表
	for (int i = 0; i < arcTagsMap[sketch_name].size(); i++)
	{
		UF_MODL_put_list_item(LineList, arcTagsMap[sketch_name][i]);
	}

	// 定义字符数组来表示拉伸角度、反向/正向的距离值（以字符串形式传递）
	char TaperAngle[32] = "";
	char opposite[32] = "";
	char towards[32] = "";

	// 设置拉伸角度为 0（无拔模角）
	sprintf_s(TaperAngle, sizeof(TaperAngle), "%f", 0.0);

	// 将反向拉伸和正向拉伸的浮点数转为字符串
	sprintf_s(opposite, sizeof(opposite), "%f", extrude_opposite);
	sprintf_s(towards, sizeof(towards), "%f", extrude_towards);

	// 形成一个数组表示两个方向的拉伸长度
	char* limit[2] = { opposite, towards };

	// 拉伸的起点（一般为草图平面原点）
	double point[3] = { 0.0, 0.0, 0.0 };

	// 原始的拉伸方向（Z轴方向）
	double origin_direction[3] = { 0.0, 0.0, 1.0 };

	// 使用 Eigen 做方向向量的矩阵变换（根据当前草图变换矩阵 R）
	Eigen::Vector3d origin_dir(origin_direction[0], origin_direction[1], origin_direction[2]);
	Eigen::Vector3d real_dir = sketchRMap[sketch_name].inverse() * origin_dir;

	// 得到最终用于拉伸的方向向量
	double direction[3] = { real_dir.x(), real_dir.y(), real_dir.z() };

	// 创建特征输出变量（返回值）
	uf_list_p_t features;

	// 执行拉伸操作，传入几何体、方向、距离、角度等参数
	int info = 1;
	info = UF_MODL_create_extruded(LineList, TaperAngle, limit, point, direction, sign, &features);

	// 如果返回值不为 0，说明失败，弹出中文错误提示
	if (info != 0)
	{
		uc1601("拉伸失败!", 1);  // uc1601 是 UFUN 的报错提示函数
	}

	// 删除临时几何体列表，释放内存
	UF_MODL_delete_list(&LineList);
}
void operation_sketches(const std::vector<tag_t>& sketch_tags, const json& sketch_data_array) {
	// 确保草图数量和 JSON 数组长度匹配
	if (sketch_tags.size() != sketch_data_array.size())
	{
		std::cerr << "Error: sketch_tags size does not match sketch_data_array size!" << std::endl;
		return;
	}
	// 遍历每个草图和对应的 JSON 数据，调用已有约束添加函数
	for (size_t i = 0; i < sketch_tags.size(); ++i)
	{
		tag_t sketch_tag = sketch_tags[i];
		const json& sketch_data = sketch_data_array[i];

		operation_sketch(sketch_tag, sketch_data);
	}
}

int main(int argc, char* argv[]) {
	int result = init(argc, argv); // 初始化函数

	if (result != 0) {
		return result; // 如果初始化失败，直接返回错误码
	}

	//读取 JSON 文件内容返回为文件对象
	json sketch_data_array = read_sketch_json(argv[1]);


	//返回草图数组多个草图
	auto tags = create_sketches(sketch_data_array);
	std::cout << "tags size: " << tags.size() << std::endl;

	//创建草图几何体
	create_sketches_geometry(tags, sketch_data_array);

	//添加约束
	add_sketches_constraints(tags, sketch_data_array);

	//操作草图
	operation_sketches(tags, sketch_data_array);

	//保存
	UF_PART_save_as(argv[2]);


	std::cout << "已完成" << std::endl;

	UF_terminate();
	printf("UG 终止\n");
	return 0;
}