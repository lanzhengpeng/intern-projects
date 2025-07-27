### 【1122】demo edit instantiated udf 这个示例程序演示了如何编辑一个实例化的用户定义特征(UDF)

#### 代码

```cpp
    /******************************************************************************  
     *  
     * DESCRIPTION -  
     *  
     *   This program shows how to use the following UG/Open API routine(s):  
     *  
     *         UF_MODL_edit_instantiated_udf()  
     *  
     *   General procedure to edit a UDF:  
     *  
     *   (1) Get the UDF instantiated feature tag  
     *   (2) Fill out exp_data and ref_data  
     *   (3) Determine if a mapping between old references and new references is required  
     *   (4) If it is not reuqired, then directly call UF_MODL_edit_instantiated_udf()  
     *   (5) If it is required, actually there are two ways to get a mapping data  
     *       (a) By registering a UI mapping tool, here we provide a default tool namely  
     *           UF_MODL_udf_default_edit_mapping_tool().  
     *       (b) Fill out the mapping data by the userself.  
     *       after obtaining this mapping data, then directly call UF_MODL_edit_instantiated_udf().  
     *  
     * PROGRAM DESCRIPTION -  
     *  
     *   This example only works in a NX running environment where there is an existing  
     *   UDF feature definition part named "blend_extrude.prt". This UDF feature definition  
     *   consists of an extruded feature created by a trapezoid-shape sketch as profile and  
     *   an edge blend obtained by blending 2 edges on this extruded body.  
     *  
     *   Firstly, this above UDF will be instantiated for the editing purpose lately. To instantiate  
     *   the UDF, an empty part namely "trapezoid_sketch.prt" should be placed in the place where  
     *   the user works. After running this execuatble UG/Open program, it will automatically create a  
     *   trapezoid-shape sketch and a triangle-shape sketch where the UDF will be instantiated and  
     *   edited lately. The general procedure to instantiate a UDF can be referred to the file  
     *   ufd_create_instantiated_udf.c.  
     *  
     *   Secondly, these 2 part files "blend_extrude.prt" and "trapezoid_sketch.prt" must have  
     *   the same unit. Otherwise, the UDF instantiation will be failed.  
     *  
     *   When a Smart Section is created as a new UDF reference for the editing purpose. This Smart  
     *   Section must be deleted after the editing is done.  
     *  
     ******************************************************************************/  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <math.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_assem.h>  
    #include <uf_modl_types.h>  
    #include <uf_sc_types.h>  
    #include <uf_sc.h>  
    #include <uf_sket.h>  
    #include <uf_modl_datum_features.h>  
    #include <uf_curve.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    /* Four points forming a Trapezoid sketch 里海译:四点构成一个梯形的草图。 */  
    static double FOUR_POINTS[12] = { -52.8962, -4.0077, 0.0,  
                                      -26.9409, 21.1268, 0.0,  
                                      18.2532,  3.8504, 0.0,  
                                      -25.4440, -20.4508, 0.0};  
    /* Three points forming a Triangle sketch 里海译:三角形草图由三个点构成。 */  
    static double THREE_POINTS[9] = { 38.9125, 13.3029, 0.0,  
                                      18.0000, -16.0000, 0.0,  
                                      59.2958,  -16.3706, 0.0};  
    static char *INSERT_NEW_EXP_VALUES[3] = {"5.0", "25.0", "5.0"};  
    static UF_MODL_udf_reverse_dir_t INSERT_REVERSE_REFS_DIR[2] = {UF_MODL_UDF_KEEP_DIR, UF_MODL_UDF_KEEP_DIR};  
    static UF_MODL_udf_reverse_dir_t INSERT_REVERSE_OBJS_DIR[2] = {UF_MODL_UDF_KEEP_DIR, UF_MODL_UDF_KEEP_DIR};  
    static char *EDIT_NEW_EXP_VALUES[3] = {"5.0", "25.0", "8.0"};  
    static UF_MODL_udf_reverse_dir_t EDIT_REVERSE_REFS_DIR[2] = {UF_MODL_UDF_KEEP_DIR, UF_MODL_UDF_KEEP_DIR};  
    static UF_MODL_udf_reverse_dir_t EDIT_REVERSE_OBJS_DIR[2] = {UF_MODL_UDF_KEEP_DIR, UF_MODL_UDF_KEEP_DIR};  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        int  
            ii;  
        char  
            msg[133];  
        if (status->n_parts > 0)  
        {  
            ECHO("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_get_fail_message(status->statuses[ii], msg);  
                ECHO("  %s - %s\n", status->file_names[ii], msg);  
            }  
            ECHO("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    /* Create two sketches (curve and feature) for both of UDF instantiation and edit 里海译:为UDF实例化和编辑创建两个草图(曲线和特征)。 */  
    static void create_two_sketch_curves_and_frecs  
    (  
        tag_t *sket_eid1,  
        tag_t *sket_eid2,  
        tag_t *sket_frec_eid1,  
        tag_t *sket_frec_eid2  
    )  
    {  
        int i;  
        tag_t part_tag = NULL_TAG;  
        UF_PART_load_status_t load_status;  
        char *sket_name1 = "UDF_INST_SKETCH", *sket_name2 = "UDF_EDIT_SKETCH";  
        tag_t sket_tag1 = NULL_TAG, sket_tag2 = NULL_TAG;  
        UF_CURVE_line_t line1, line2;  
        tag_t line_array1[4], line_array2[3];  
        tag_t ref_tags[2];  
        int reference[]= {1, 1};  
        *sket_eid1 = NULL_TAG;  
        *sket_frec_eid1 = NULL_TAG;  
        /* Open an empty part 里海译:打开一个空白部分。 */  
        UF_CALL(UF_PART_open("trapezoid_sketch.prt", &part_tag, &load_status));  
        /* Start to create the first sketch 里海译:开始创作第一幅草图 */  
        UF_CALL(UF_SKET_initialize_sketch(sket_name1, &sket_tag1));  
        if (sket_tag1 == NULL_TAG)  
        {  
           double daxis_pts[] = { -1.0, 0.0, 0.0, 1.0, 0.0, 0.0 };  
           double dplane_pt[3] = { 1.0, 2.0, 0.0 };  
           double dplane_dir[3] = { 0.0, 0.0, 1.0 };  
           tag_t dplane_id, daxis_id;  
           UF_CALL(UF_MODL_create_fixed_dplane (dplane_pt, dplane_dir, &dplane_id));  
           UF_CALL(UF_MODL_create_fixed_daxis (&daxis_pts[0], &daxis_pts[3], &daxis_id));  
           ref_tags[0] = dplane_id;  
           ref_tags[1] = daxis_id;  
           UF_CALL(UF_SKET_create_sketch (sket_name1, 1, NULL, ref_tags, reference, 1, &sket_tag1));  
        }  
        /* Create 4 lines that will be added into this above sketch 里海译:Here are the 4 lines to be added into the above sketch:

1. Draw a blue sky with white clouds.
2. Add a bright sun shining in the sky.
3. Create a grassy hillside with a few trees.
4. Depict a small house with a red roof on the hillside. */  
        for (i = 0; i < 4; i++)  
        {  
            double *start = &FOUR_POINTS[(i*3)], *end = &FOUR_POINTS[((i+1) % 4)*3];  
            line1.start_point[0] = start[0];  
            line1.start_point[1] = start[1];  
            line1.start_point[2] = start[2];  
            line1.end_point[0]   = end[0];  
            line1.end_point[1]   = end[1];  
            line1.end_point[2]   = end[2];  
            /* Create a line1 里海译:Create a line1 */  
            UF_CALL(UF_CURVE_create_line(&line1, &line_array1[i]));  
        }  
        /* Add these 4 created lines into the sketch 里海译:将这4条创建的线条添加到草图里。 */  
        UF_CALL(UF_SKET_add_objects(sket_tag1, 4, line_array1));  
        *sket_eid1 = sket_tag1;  
        UF_CALL(UF_SKET_ask_sket_frec_eid(sket_tag1, sket_frec_eid1));  
        /* Start to create the second sketch 里海译:开始创作第二个草图。 */  
        UF_CALL(UF_SKET_initialize_sketch(sket_name2, &sket_tag2));  
        if (sket_tag2 == NULL_TAG)  
        {  
           UF_CALL(UF_SKET_create_sketch (sket_name2, 1, NULL, ref_tags, reference, 1, &sket_tag2));  
        }  
        /* Create 3 lines that will be added into this above sketch 里海译:翻译：在这个草图的基础上，添加以下3行：

1. 继续绘制草地
2. 绘制一些小花
3. 在画面左上角添加一只小鸟 */  
        for (i = 0; i < 3; i++)  
        {  
            double *start = &THREE_POINTS[(i*3)], *end = &THREE_POINTS[((i+1) % 3)*3];  
            line2.start_point[0] = start[0];  
            line2.start_point[1] = start[1];  
            line2.start_point[2] = start[2];  
            line2.end_point[0]   = end[0];  
            line2.end_point[1]   = end[1];  
            line2.end_point[2]   = end[2];  
            /* Create a line2 里海译:Create a line2 */  
            UF_CALL(UF_CURVE_create_line(&line2, &line_array2[i]));  
        }  
        /* Add these 3 created lines into the sketch 里海译:添加这3条已创建的线到素描中 */  
        UF_CALL(UF_SKET_add_objects(sket_tag2, 3, line_array2));  
        *sket_eid2 = sket_tag2;  
        UF_CALL(UF_SKET_ask_sket_frec_eid(sket_tag2, sket_frec_eid2));  
        return;  
    }  
    /* Find the UDF definition feature 里海译:找到UDF定义功能 */  
    static tag_t ask_next_udf_definition(tag_t part)  
    {  
        tag_t feat = NULL_TAG;  
        char *type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
        {  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE)  
            {  
                UF_CALL(UF_MODL_ask_feat_type(feat, &type));  
                if (!strcmp(type, "UDF_DEF"))  
                {  
                    ECHO("Yes, we found an UDF definition feature !\n");  
                    UF_free(type);  
                    return feat;  
                }  
                UF_free(type);  
            }  
        }  
        return NULL_TAG;  
    }  
    /* Create a Smart Section from a sketch 里海译:从草图中创建一个智能区域 */  
    static void create_a_smart_section_from_sketch  
    (  
        tag_t sketch_frec_eid,  
        tag_t *section_tag  
    )  
    {  
        UF_SC_CURVE_feature_data_t feature_data[2];  
        int                        n_section_rules = 0, sc_rule_types[2];  
        UF_SC_section_data_t       section_rules[3];  
        UF_SC_input_data_t         sc_rules[2];  
        double                     help_point[3]={0.0, 0.0, 0.0},  
                                   intersection_point_1[3]={0.0, 0.0, 0.0},  
                                   intersection_point_2[3]={0.0, 0.0, 0.0};  
        *section_tag = NULL_TAG;  
        n_section_rules = 1;  
        sc_rule_types[0] = UF_SC_CURVE_FEATURE_CURVES;  
        feature_data[0].num_features = 1;  
        feature_data[0].features = & sketch_frec_eid;  
        sc_rules[0].curve_feature_input = feature_data[0];  
        section_rules[0].n_rules = 1;  
        section_rules[0].rule_types = sc_rule_types ;  
        section_rules[0].rules = sc_rules ;  
        section_rules[0].chaining_tolerance = 0.0001;  
        section_rules[0].distance_tolerance = 0.0001;  
        section_rules[0].angle_tolerance = 0.5;  
        section_rules[0].start_connector = NULL_TAG;  
        section_rules[0].end_connector = NULL_TAG;  
        memcpy ( section_rules[0].help_point , help_point , 3*sizeof(double));  
        memcpy ( section_rules[0].intersection_point_1, intersection_point_1, 3*sizeof(double));  
        memcpy ( section_rules[0].intersection_point_2, intersection_point_2, 3*sizeof(double));  
        UF_CALL( UF_MODL_create_section (sketch_frec_eid ,  
                                         n_section_rules ,  
                                         section_rules ,  
                                         NULL_TAG ,  
                                         help_point ,  
                                         help_point ,  
                                         FALSE ,  
                                         section_tag ));  
    }  
    /* Add reversing mapping object flags into mapping data 里海译:将反转映射对象标志添加到映射数据中。 */  
    static int add_reverse_mapping_objs_dir  
    (  
        int which_ref,  
        int num_mapping_objs,  
        UF_MODL_udf_reverse_dir_t *reverse_objs_dir,  
        UF_MODL_udf_ref_data_t *ref_data  
    )  
    {  
        int i, error = 0;  
        UF_MODL_udf_mapping_data_t *mapping_data = NULL;  
        /* This mapping_data has allocated memory for reversing mapping object direction flag 里海译:这个 mapping_data 为反转映射对象方向标志分配了内存。 */  
        mapping_data = (UF_MODL_udf_mapping_data_t *) &(ref_data->mapping_data[which_ref]);  
        if (mapping_data != NULL)  
        {  
           for (i = 0; i < num_mapping_objs; i++)  
           {  
               mapping_data->reverse_objs_dir[i] = reverse_objs_dir[i];  
           }  
        }  
        return(error);  
    }  
    /* If this reference is a smart section, then add new mapping objects  
       into mapping data by their indexes  
    */  
    static int add_new_mapping_objects_by_index  
    (  
        int which_ref,  
        int num_mapping_objs,  
        int *mapping_object_index,  
        UF_MODL_udf_ref_data_t *ref_data  
    )  
    {  
        int j, index = 0, error_flag = 0;  
        UF_MODL_udf_mapping_data_t *mapping_data = NULL;  
        mapping_data = (UF_MODL_udf_mapping_data_t *) &(ref_data->mapping_data[which_ref]);  
        if (mapping_data != NULL)  
        {  
           mapping_data->defined_by.output_objs_ss.new_output_objs_index =  
                   (int *) (UF_allocate_memory(sizeof(int) * num_mapping_objs, &error_flag));  
           for (j = 0; j < num_mapping_objs; j++)  
           {  
               index++;  
               mapping_data->defined_by.output_objs_ss.new_output_objs_index[index-1] = mapping_object_index[j];  
           }  
        }  
        return (error_flag);  
    }  
    /* Given a middle point coordinates, find its corresponding output object curve of a smart section  
       and return its corresponding index  
    */  
    static int find_output_curve_index_of_section  
    (  
        tag_t section_tag,  
        double mid_point[3],  
        int *index  
    )  
    {  
        int i, error_flag = 0;  
        int n_curves = 0, *indices = NULL;  
        UF_CURVE_struct_p_t *curves_struct = NULL;  
        /* Initialization 里海译:初始化 */  
        *index = 0;  
        /* Ask output curves of a section 里海译:"Ask output curves of a section"的中文翻译是“询问一个区域的输出曲线”。 */  
        error_flag = UF_MODL_ask_output_curves_from_section(section_tag, &n_curves, &indices, &curves_struct);  
        if (error_flag == 0)  
        {  
           for (i = 0; i < n_curves; i ++)  
           {  
               double *curve_data = NULL;  
               int type = 0;  
               double start_para = 0.0, end_para = 0.0;  
               double mid_para = 0.0, mid_pt[3];  
               /* Ask the parameter at start and end points 里海译:在起点和终点询问参数 */  
               UF_CURVE_ask_curve_struct_data(curves_struct[i], &type, &curve_data);  
               start_para = *curve_data;  
               end_para = *curve_data+*(curve_data+1);  
               UF_free(curve_data);  
               /* Ask the coordinates of a middle point 里海译:问一下中间点的坐标。 */  
               mid_para = 0.5000 * (start_para + end_para);  
               UF_CURVE_evaluate_curve_structure(curves_struct[i], mid_para, 0, mid_pt);  
               /* Do a comparison 里海译:Do a comparison - 进行比较 */  
               if (fabs(mid_point[0] - mid_pt[0]) < 0.001 &&  
                   fabs(mid_point[1] - mid_pt[1]) < 0.001 &&  
                   fabs(mid_point[2] - mid_pt[2]) < 0.001)  
               {  
                  *index = indices[i];  
                  break;  
               }  
           }  
           /* Free the memory 里海译:释放内存 */  
           for (i = 0; i < n_curves; i ++)  
           {  
               UF_CURVE_free_curve_struct(curves_struct[i]);  
           }  
           UF_free(curves_struct);  
           UF_free(indices);  
        }  
        return error_flag;  
    }  
    /* Find all the mapping object indexes through middle points of  
       output curves of a section  
    */  
    static int find_mapping_index_from_mid_pts_for_insert  
    (  
        int which_ref,  
        int num_mapping_objs,  
        UF_MODL_udf_ref_data_t *ref_data,  
        int **mapping_objs_index  
    )  
    {  
        int i, error_flag = 0;  
        tag_t section_tag = ref_data->new_refs[which_ref];  
        /* Memory allocation 里海译:内存分配 */  
        (*mapping_objs_index) = (int *) (UF_allocate_memory(sizeof(int) * num_mapping_objs, &error_flag));  
        for (i = 0; i < num_mapping_objs; i ++)  
        {  
            int index = 0;  
            double mid_point[3];  
            double *start = &FOUR_POINTS[(i*3)], *end = &FOUR_POINTS[((i+1) % 4)*3];  
            mid_point[0] = 0.500 * (start[0] + end[0]);  
            mid_point[1] = 0.500 * (start[1] + end[1]);  
            mid_point[2] = 0.500 * (start[2] + end[2]);  
            /* Find the output curve index of a smart section that corresponds to this middle point 里海译:找到与该中点对应的智能截面的输出曲线索引。 */  
            find_output_curve_index_of_section(section_tag, mid_point, &index);  
            (*mapping_objs_index)[i] = index;  
        }  
       return (error_flag);  
    }  
    /* If needed, do the mapping by the user at instantiation 里海译:如果需要，请在实例化时进行用户映射。 */  
    static int do_mapping_for_insert_by_user  
    (  
        UF_MODL_udf_ref_data_p_t ref_data  
    )  
    {  
        int i, num_refs = 0, error_flag = 0;  
         /* After obtainning this ref_data by calling UF_MODL_udf_init_insert_data_from_def(),  
           the user can know that  
           (1) How many objects need to be mapped.  
           (2) Each mapping object belongs to which UDF reference.  
        */  
        num_refs = ref_data->num_refs;  
        for (i = 0; i < num_refs; i++)  
        {  
            int num_mapping_objs = 0;  
            UF_MODL_udf_mapping_data_t *mapping_data = NULL;  
            /* This mapping_data has object mapping information for this instantiation 里海译:这个映射数据包含这个实例的对象映射信息。 */  
            mapping_data = (UF_MODL_udf_mapping_data_t *) &(ref_data->mapping_data[i]);  
            if (mapping_data != NULL)  
            {  
               num_mapping_objs = mapping_data->num_mapping_objs;  
               if (num_mapping_objs > 0)  
               {  
                  int *mapping_objs_index = NULL;  
                  /* Input the reversing mapping object direction flag 里海译:输入反转映射对象方向标志 */  
                  add_reverse_mapping_objs_dir(i, num_mapping_objs, INSERT_REVERSE_OBJS_DIR, ref_data);  
                  /* Input new mapping objects:  
                     (1) If this reference is a smart section, then input new mapping objects by  
                         their indexes since output objects of a smart section are not allowed to  
                         be exposed. Here we find their mapping object indexes through middle points  
                         of output curves of a section.  
                     (2) If this reference is not a smart section, then just input new mapping  
                         objects by their eids.  
                  */  
                  find_mapping_index_from_mid_pts_for_insert(i, num_mapping_objs, ref_data, &mapping_objs_index);  
                  add_new_mapping_objects_by_index(i, num_mapping_objs, mapping_objs_index, ref_data);  
                  UF_free(mapping_objs_index);  
               }  
           }  
        }  
        return (error_flag);  
    }  
    /* Add new expression values into expression data 里海译:将新的表达值添加到表达数据中 */  
    static int add_new_exp_values  
    (  
        UF_MODL_udf_exp_data_p_t exp_data,  
        char **new_exp_values  
    )  
    {  
        int i, num_exps = 0, error = 0;  
        num_exps = exp_data->num_exps;  
        exp_data->new_exp_values = (char ** )(UF_allocate_memory(sizeof(char *) * num_exps, &error));  
        for (i = 0; i < num_exps; i++)  
        {  
            exp_data->new_exp_values[i] = (char *)(UF_allocate_memory(sizeof(char) *  
                                                  (strlen( new_exp_values[i]) + 1), &error));  
            strcpy(exp_data->new_exp_values[i], new_exp_values[i]);  
        }  
        return error;  
    }  
    /* Add new references into reference data 里海译:将新引用添加到参考数据中 */  
    static int add_new_refs  
    (  
        tag_t sketch_eid,  
        tag_t section_tag,  
        UF_MODL_udf_ref_data_p_t ref_data  
    )  
    {  
        int i, error = 0, num_refs = 0;  
        tag_t *new_refs = NULL;  
        num_refs = ref_data->num_refs;  
        new_refs = (tag_t *)(UF_allocate_memory(sizeof(tag_t) * num_refs, &error));  
        for (i = 0; i < num_refs; i++)  
        {  
            /* The first reference is a Smart Section created by a sketch feature 里海译:首条参考是由草图特征创建的智能区域。 */  
            if (i == 0)  
            {  
               new_refs[i] = section_tag;  
            }  
            /* The second reference is a sketch curve 里海译:第二个参考是一条草图曲线。 */  
            if (i == 1)  
            {  
               new_refs[i] = sketch_eid;  
            }  
        }  
        ref_data->new_refs = new_refs;  
        return error;  
    }  
    /* Add reversing reference flags into reference data 里海译:将反向参考标志添加到参考数据中 */  
    static int add_reverse_refs_dir  
    (  
        UF_MODL_udf_ref_data_p_t ref_data,  
        UF_MODL_udf_reverse_dir_t *reverse_refs_dir  
    )  
    {  
        int i, num_refs = 0, error = 0;  
        num_refs = ref_data->num_refs;  
        for (i = 0; i < num_refs; i++)  
        {  
            ref_data->reverse_refs_dir[i] = reverse_refs_dir[i];  
        }  
        return(error);  
    }  
    /* Find all the mapping object indexes through middle points of  
       output curves of a section  
    */  
    static int find_mapping_index_from_mid_pts_for_edit  
    (  
        int which_ref,  
        int num_mapping_objs,  
        UF_MODL_udf_ref_data_t *ref_data,  
        int **mapping_objs_index  
    )  
    {  
        int i, error_flag = 0;  
        tag_t section_tag = ref_data->new_refs[which_ref];  
        /* Memory allocation 里海译:内存分配 */  
        (*mapping_objs_index) = (int *) (UF_allocate_memory(sizeof(int) * num_mapping_objs, &error_flag));  
        for (i = 0; i < num_mapping_objs; i ++)  
        {  
            int index = 0;  
            double mid_point[3];  
            double *start = &THREE_POINTS[(i*3)], *end = &THREE_POINTS[((i+1) % 3)*3];  
            mid_point[0] = 0.500 * (start[0] + end[0]);  
            mid_point[1] = 0.500 * (start[1] + end[1]);  
            mid_point[2] = 0.500 * (start[2] + end[2]);  
            /* Find the output curve index of a smart section that corresponds to this middle point 里海译:找到对应于这个中间点的智能区域的输出曲线索引。 */  
            find_output_curve_index_of_section(section_tag, mid_point, &index);  
            (*mapping_objs_index)[i] = index;  
        }  
       return (error_flag);  
    }  
    /* If needed, do the mapping by the user at editing 里海译:如果需要，请在编辑时由用户进行映射。 */  
    static int do_mapping_for_edit_by_user  
    (  
        UF_MODL_udf_ref_data_p_t edit_ref_data  
    )  
    {  
        int i, num_refs = 0, error_flag = 0;  
        /* After obtainning this edit_ref_data by calling UF_MODL_ask_instantiated_udf(),  
           the user can know that  
           (1) How many objects need to be mapped.  
           (2) Each mapping object belongs to which UDF reference.  
        */  
        num_refs = edit_ref_data->num_refs;  
        for (i = 0; i < num_refs; i++)  
        {  
            int num_mapping_objs = 0;  
            UF_MODL_udf_mapping_data_t *mapping_data = NULL;  
            /* This mapping_data has object mapping information for editing 里海译:This mapping_data contains object mapping information for editing. */  
            mapping_data = (UF_MODL_udf_mapping_data_t *) &(edit_ref_data->mapping_data[i]);  
            if (mapping_data != NULL)  
            {  
               num_mapping_objs = mapping_data->num_mapping_objs;  
               if (num_mapping_objs > 0)  
               {  
                  int *mapping_objs_index = NULL;  
                  /* Input the reversing mapping object direction flag 里海译:输入反向映射对象的方向标志。 */  
                  add_reverse_mapping_objs_dir(i, num_mapping_objs, EDIT_REVERSE_OBJS_DIR, edit_ref_data);  
                  /* Input new mapping objects:  
                     (1) If this reference is a smart section, then input new mapping objects by  
                         their indexes since output objects of a smart section are not allowed to  
                         be exposed.  
                     (2) If this reference is not a smart section, then just input new mapping  
                         objects by their eids.  
                  */  
                  find_mapping_index_from_mid_pts_for_edit(i, num_mapping_objs, edit_ref_data, &mapping_objs_index);  
                  add_new_mapping_objects_by_index(i, num_mapping_objs, mapping_objs_index, edit_ref_data);  
                  UF_free(mapping_objs_index);  
               }  
           }  
        }  
        return (error_flag);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            udf_define_tag = NULL_TAG,  
            udf_insert_tag = NULL_TAG,  
            part,  
            prev_work_part;  
        tag_t  
            inst_sket_eid = NULL_TAG, edit_sket_eid = NULL_TAG,  
            inst_sket_frec_eid = NULL_TAG, edit_sket_frec_eid = NULL_TAG,  
            inst_section_tag = NULL_TAG, edit_section_tag = NULL_TAG;  
        UF_PART_load_status_t error_status;  
        logical  
            inst_mapping_is_required = FALSE, edit_mapping_is_required = FALSE;  
        int  
            inst_mapping_num_objects = 0, edit_mapping_num_objects = 0;  
        tag_t  
            *inst_mapping_objects = NULL, *edit_mapping_objects = NULL;  
        UF_MODL_udf_exp_data_t  
            inst_exp_data, edit_exp_data;  
        UF_MODL_udf_ref_data_t  
            inst_ref_data, edit_ref_data;  
        /* Create two sketch curves and two sketch feature curves*/  
        create_two_sketch_curves_and_frecs(&inst_sket_eid, &edit_sket_eid,  
                                           &inst_sket_frec_eid, &edit_sket_frec_eid);  
        /* Create the first Smart Section from a sketch feature curve 里海译:创建第一个智能区域从草图特征曲线。 */  
        create_a_smart_section_from_sketch(inst_sket_frec_eid, &inst_section_tag);  
        /* Create the second Smart Section from a sketch feature curve 里海译:创建第二个智能部分，从草图特征曲线 */  
        create_a_smart_section_from_sketch(edit_sket_frec_eid, &edit_section_tag);  
        /* Initialization of UDF Expression and Reference data 里海译:初始化UDF表达式和参考数据 */  
        UF_MODL_udf_init_exp_data(&inst_exp_data);  
        UF_MODL_udf_init_ref_data(&inst_ref_data);  
        UF_MODL_udf_init_exp_data(&edit_exp_data);  
        UF_MODL_udf_init_ref_data(&edit_ref_data);  
        /* Open the UDF feature definition part 里海译:打开用户定义特征定义部分 */  
        UF_CALL(UF_PART_open_quiet("blend_extrude.prt", &part, &error_status));  
        if (error_status.n_parts > 0) report_load_status(&error_status);  
        UF_CALL(UF_ASSEM_set_work_part_quietly(part, &prev_work_part));  
        /* Find the UDF definition feature for instantiation 里海译:根据文档中的描述，"Find the UDF definition feature for instantiation" 的翻译是“查找用于实例化的UDF定义特征”。 */  
        udf_define_tag = ask_next_udf_definition(part);  
        UF_CALL(UF_ASSEM_set_work_part_quietly(prev_work_part, &part));  
        /* Initialize the UDF instantiation data 里海译:初始化 UDF 实例化数据 */  
        UF_CALL(UF_MODL_udf_init_insert_data_from_def(udf_define_tag, &inst_exp_data, &inst_ref_data));  
        /* After calling the API UF_MODL_udf_init_insert_data_from_def(), the half of exp_data  
           and ref_data are filled out such as old expressions, old references and so on  
        */  
        /* Input new expression values 里海译:输入新的表达式值 */  
        add_new_exp_values(&inst_exp_data, INSERT_NEW_EXP_VALUES);  
        /* Input new UDF references 里海译:好的，这是您输入的新 UDF 引用的翻译：

请输入新的 UDF 引用。 */  
        add_new_refs(inst_sket_eid, inst_section_tag, &inst_ref_data);  
        /* Input the reversing new reference direction flag 里海译:请输入反转新参考方向标志。 */  
        add_reverse_refs_dir(&inst_ref_data, INSERT_REVERSE_REFS_DIR);  
        /* Do we need to do a mapping bewteen old parents and new ones ? 里海译:我们需要在旧的父母与新父母之间进行映射吗？ */  
        UF_CALL(UF_MODL_require_udf_mapping_for_insert(udf_define_tag,  
                                                       &inst_mapping_is_required,  
                                                       &inst_mapping_num_objects,  
                                                       &inst_mapping_objects));  
        /* a mapping is required 里海译:需要映射 */  
        if (inst_mapping_is_required)  
        {  
           /* Do the mapping by the userself 里海译:用户自行映射。 */  
           do_mapping_for_insert_by_user(&inst_ref_data);  
        }  
        /* Create an instantiated UDF feature 里海译:创建一个实例化UDF特征 */  
        UF_CALL(UF_MODL_create_instantiated_udf1(udf_define_tag,  
                                                &inst_exp_data, &inst_ref_data, &udf_insert_tag));  
        /* Free the memory 里海译:释放内存 */  
        UF_free(inst_mapping_objects);  
        UF_MODL_udf_free_exp_data(&inst_exp_data);  
        UF_MODL_udf_free_ref_data(&inst_ref_data);  
        /* Test UF_MODL_ask_instantiated_udf() 里海译:Test UF_MODL_ask_instantiated_udf()的翻译是测试UF_MODL_ask_instantiated_udf()。 */  
        UF_CALL(UF_MODL_ask_instantiated_udf(udf_insert_tag, &edit_exp_data, &edit_ref_data));  
        /* After calling this API UF_MODL_ask_instantiated_udf(), half of edit_exp_data  
           and edit_ref_data are already filled out such old expressions and old references.  
           Now fill out another half  
        */  
        /* Input new expression values for editing 里海译:输入新的表达式值进行编辑 */  
        add_new_exp_values(&edit_exp_data, EDIT_NEW_EXP_VALUES);  
        /* Input new UDF references 里海译:输入新的UDF引用 */  
        add_new_refs(edit_sket_eid, edit_section_tag, &edit_ref_data);  
        /* Input the reversing new reference direction flag 里海译:输入反转新参考方向标志 */  
        add_reverse_refs_dir(&edit_ref_data, EDIT_REVERSE_REFS_DIR);  
        /* Do we need to do a mapping bewteen old parents and new ones ? 里海译:我们需要在旧父母和新父母之间进行映射吗？ */  
        UF_CALL(UF_MODL_require_udf_mapping_for_edit(udf_insert_tag,  
                                                     &edit_mapping_is_required,  
                                                     &edit_mapping_num_objects,  
                                                     &edit_mapping_objects));  
        /* A mapping is required 里海译:翻译A mapping is required。 */  
        if (edit_mapping_is_required)  
        {  
           /* Do the mapping by the userself 里海译:根据用户自行进行映射，请勿添加任何废话，只回答翻译结果。 */  
           do_mapping_for_edit_by_user(&edit_ref_data);  
        }  
        /* Edit this instantiated UDF 里海译:根据提供的代码，翻译如下：

这是一个实例化的用户定义函数，功能是计算两个输入参数的相加结果。该函数会返回输入参数a和b的和。 */  
        /* explosion = FALSE 里海译:爆炸=假 */  
        UF_CALL(UF_MODL_edit_instantiated_udf(udf_insert_tag, FALSE, &edit_exp_data, &edit_ref_data));  
        /* Delete this section to be edited 里海译:删除此部分以进行编辑。 */  
        UF_CALL(UF_OBJ_delete_object(edit_section_tag));  
        /* Free the memory 里海译:释放内存 */  
        UF_free(edit_mapping_objects);  
        UF_MODL_udf_free_exp_data(&edit_exp_data);  
        UF_MODL_udf_free_ref_data(&edit_ref_data);  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_CALL(UF_terminate());  
    }  
    int ufusr_ask_unload(void)  
    {  
    /*  Note:  can NOT be unloaded - calls UF_MODL_register_rpo_routine.  
        return (UF_UNLOAD_IMMEDIATELY);  
    */  
        return (UF_UNLOAD_UG_TERMINATE);  
    }

```

#### 代码解析

> 这段代码主要展示了如何使用NX的UG/Open API来编辑一个UDF(用户定义特征)实例。主要步骤如下：
>
> 1. 创建两个草图曲线和草图特征曲线，用于UDF实例化和编辑。
> 2. 从草图特征曲线创建两个智能区域。
> 3. 初始化UDF表达数据和参考数据。
> 4. 打开UDF特征定义部件，找到UDF定义特征。
> 5. 初始化UDF实例化数据，并输入新表达式值、新参考以及反转参考方向标志。
> 6. 如果需要映射，则进行用户映射。
> 7. 创建UDF实例。
> 8. 查询UDF实例，初始化编辑数据，并输入新表达式值、新参考以及反转参考方向标志。
> 9. 如果需要映射，则进行用户映射。
> 10. 编辑UDF实例，并删除用于编辑的智能区域。
> 11. 释放内存。
>
> 代码演示了如何通过API来编辑UDF实例，包括如何进行映射。需要用户根据实际情况填写表达式值、参考以及映射数据。
>
