### 【0923】create square pocket with taper by outline no attach 创建无附着轮廓渐变方孔

#### 代码

```cpp
    /*HEAD CREATE_SQUARE_POCKET_WITH_TAPER_BY_OUTLINE_NO_ATTACH CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中提供的信息，可以翻译为：

注意：UF_print_syslog是V18版本中新增的。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    static void do_it(void)  
    {  
        int    units= UF_PART_METRIC;  
        int    num_faces,ii;  
        char   *part_name="pocket";  
        char   radius[133] = {"0.0"};  
        char   *edge_lens[] = {"100.0","50.0","100.0"};  
        double origin[3]={0.0,0.0,0.0};  
        double face_param[2],face_point[3],face_u_deriv1[3];  
        double face_v_deriv1[3],face_u_deriv2[3],face_v_deriv2[3];  
        double face_normal[3],face_radii[2];  
        uf_list_p_t face_list;  
        UF_CURVE_line_t line1, line2, line3, line4;  
        UF_MODL_faces_t placement_faces,selected_floor_faces;  
        UF_MODL_offset_trans_faces_t floor_faces;  
        UF_MODL_pocketpad_outline_t placement_outline;  
        UF_MODL_project_curves_t outline_curves;  
        UF_MODL_parm_t placement_radius,floor_radius,corner_radius;  
        UF_MODL_vector_t placement_proj_vector =  
                    {0,UF_MODL_VECTOR_DIRECTION,  
                     {0,1,0}};  
        UF_MODL_parm_t taper_angle[4];  
        UF_MODL_law_spine_t law_spine[4];  
        tag_t  part_tag,block_tag;  
        tag_t  place_face[1],floor_face[1],general_pocket_tag;  
        tag_t  placement_curves[4],face_tag;  
        UF_PART_new(part_name, units, &part_tag);  
    /* create the block 里海译:创建区块 */  
        UF_CALL(UF_MODL_create_block1(UF_NULLSIGN,  
                              origin,  
                              edge_lens,  
                              &block_tag));  
    /* create the placement outline curves 里海译:创建放置轮廓曲线 */  
    /* create a straight line 里海译:划直线 */  
        line1.start_point[0] =  85.0;  
        line1.start_point[1] =  0.0;  
        line1.start_point[2] =  70.0;  
        line1.end_point[0] =  85.0;  
        line1.end_point[1] =  0.0;  
        line1.end_point[2] =  50.0;  
        UF_CALL(UF_CURVE_create_line(&line1, &(placement_curves[0])));  
    /* create a straight line 里海译:划直线 */  
        line2.start_point[0] =  85.0;  
        line2.start_point[1] =  0.0;  
        line2.start_point[2] =  50.0;  
        line2.end_point[0] =  25.0;  
        line2.end_point[1] =  0.0;  
        line2.end_point[2] =  50.0;  
        UF_CALL(UF_CURVE_create_line(&line2, &(placement_curves[1])));  
    /* create a straight line 里海译:创建一条直线 */  
        line3.start_point[0] =  25.0;  
        line3.start_point[1] =  0.0;  
        line3.start_point[2] =  50.0;  
        line3.end_point[0] =  25.0;  
        line3.end_point[1] =  0.0;  
        line3.end_point[2] =  70.0;  
        UF_CALL(UF_CURVE_create_line(&line3, &(placement_curves[2])));  
    /* create a straight line 里海译:创建一条直线 */  
        line4.start_point[0] =  25.0;  
        line4.start_point[1] =  0.0;  
        line4.start_point[2] =  70.0;  
        line4.end_point[0] =  85.0;  
        line4.end_point[1] =  0.0;  
        line4.end_point[2] =  70.0;  
        UF_CALL(UF_CURVE_create_line(&line4, &(placement_curves[3])));  
    /* determine the placement face from the block,it is the    里海译:根据块确定放置面，它就是... */  
    /* face whose normal is mostly in the positive y_direction, 里海译:正面法向量主要在正y方向的face */  
    /* the floor face is the face whose normal is mostly   里海译:地面面是法线主要指向下方的面。 */  
    /* in the negative y_direction                              里海译:in the negative y_direction的翻译是“在负y方向”。 */  
        UF_MODL_ask_feat_faces(block_tag,  
                               &face_list);  
        UF_MODL_ask_list_count(face_list,  
                               &num_faces);  
        face_param[0] = 0.5;  
        face_param[1] = 0.5;  
        for (ii = 0;ii < num_faces;ii++)  
        {  
          UF_MODL_ask_list_item(face_list,  
                                ii,  
                                &face_tag);  
          UF_MODL_ask_face_props(face_tag,  
                                 face_param,  
                                 face_point,  
                                 face_u_deriv1,face_v_deriv1,  
                                 face_u_deriv2,face_v_deriv2,  
                                 face_normal,face_radii);  
          if (face_normal[1] > 0.75)  
            place_face[0] = face_tag;  
          if (face_normal[1] < -0.75)  
            floor_face[0] = face_tag;  
        }  
        UF_MODL_delete_list(&face_list);  
    /* set up input data to create the pocket 里海译:设置输入数据以创建口袋 */  
    /* set up data for placement faces 里海译:设置好放置面数据。 */  
        placement_faces.number_of_faces = 1;  
        placement_faces.faces = place_face;  
        placement_faces.face_extension =  
                            UF_MODL_FACE_EXTENSION_NONE;  
    /* set up data for floor faces 里海译:设置楼层面数据 */  
        selected_floor_faces.number_of_faces = 1;  
        selected_floor_faces.faces = floor_face;  
        selected_floor_faces.face_extension =  
                            UF_MODL_FACE_EXTENSION_NONE;  
        floor_faces.faces = &selected_floor_faces;  
        floor_faces.method_type = UF_MODL_OFFSET_OF_FACES;  
        strcpy(floor_faces.distance,"0.0");  
        floor_faces.direction = NULL;  
    /* set up data for placement outline 里海译:设置数据放置轮廓的翻译是：为放置轮廓设置数据 */  
        outline_curves.number_of_curves = 4;  
        outline_curves.curves = placement_curves;  
        outline_curves.curve_projection_type =  
                    UF_MODL_PROJ_ALONG_VECTOR;  
        outline_curves.curve_projection_vector =  
                            &placement_proj_vector;  
        placement_outline.outline_type = UF_MODL_SINGLE_OUTLINE_NO_ATTACH;  
        placement_outline.defined_by.single_outline_corner.  
            outline_curves = &outline_curves;  
        placement_outline.defined_by.single_outline_corner.  
            curves_represent = UF_MODL_THEORETICAL_INTERSECT;  
        placement_outline.defined_by.single_outline_corner.  
            outline_represents = UF_MODL_SHAPE_AT_PLACEMENT;  
        placement_outline.defined_by.single_outline_corner.  
            taper_angle = taper_angle;  
        taper_angle[0].method_type = UF_MODL_PARM_LAW_SPINE;  
        taper_angle[0].defined_by.law_spine.law = &law_spine[0];  
        UF_MODL_init_string_list(&law_spine[0].spine_profile);  
        UF_MODL_create_string_list(1, 1, &law_spine[0].spine_profile);  
        law_spine[0].spine_profile.string[0] = 1;  
        law_spine[0].spine_profile.dir[0] = 1;  
        law_spine[0].spine_profile.id[0] = placement_curves[0];  
        law_spine[0].method_type = UF_MODL_LAW_CONSTANT;  
        strcpy(law_spine[0].defined_by.constant.value, "5.0");  
        taper_angle[1].method_type = UF_MODL_PARM_LAW_SPINE;  
        taper_angle[1].defined_by.law_spine.law = &law_spine[1];  
        UF_MODL_init_string_list(&law_spine[1].spine_profile);  
        UF_MODL_create_string_list(1, 1, &law_spine[1].spine_profile);  
        law_spine[1].spine_profile.string[0] = 1;  
        law_spine[1].spine_profile.dir[0] = 1;  
        law_spine[1].spine_profile.id[0] = placement_curves[1];  
        law_spine[1].method_type = UF_MODL_LAW_CONSTANT;  
        strcpy(law_spine[1].defined_by.constant.value, "7.0");  
        taper_angle[2].method_type = UF_MODL_PARM_LAW_SPINE;  
        taper_angle[2].defined_by.law_spine.law = &law_spine[2];  
        UF_MODL_init_string_list(&law_spine[2].spine_profile);  
        UF_MODL_create_string_list(1, 1, &law_spine[2].spine_profile);  
        law_spine[2].spine_profile.string[0] = 1;  
        law_spine[2].spine_profile.dir[0] = 1;  
        law_spine[2].spine_profile.id[0] = placement_curves[2];  
        law_spine[2].method_type = UF_MODL_LAW_CONSTANT;  
        strcpy(law_spine[2].defined_by.constant.value, "6.0");  
        taper_angle[3].method_type = UF_MODL_PARM_LAW_SPINE;  
        taper_angle[3].defined_by.law_spine.law = &law_spine[3];  
        UF_MODL_init_string_list(&law_spine[3].spine_profile);  
        UF_MODL_create_string_list(1, 1, &law_spine[3].spine_profile);  
        law_spine[3].spine_profile.string[0] = 1;  
        law_spine[3].spine_profile.dir[0] = 1;  
        law_spine[3].spine_profile.id[0] = placement_curves[3];  
        law_spine[3].method_type = UF_MODL_LAW_CONSTANT;  
        strcpy(law_spine[3].defined_by.constant.value, "8.0");  
        placement_outline.defined_by.single_outline_corner.  
            taper_relative_to = UF_MODL_TAPER_BY_OUTLINE_FACE_NORMALS;  
        placement_outline.defined_by.single_outline_corner.  
            taper_vector = NULL;  
        placement_outline.defined_by.single_outline_corner.  
            corner_radius = &corner_radius;  
        corner_radius.method_type = UF_MODL_PARM_CONSTANT;  
        strcpy(corner_radius.defined_by.constant.value, "0.0");  
    /* set up placement and floor radius 里海译:设置位置和楼面半径 */  
        placement_radius.method_type = UF_MODL_PARM_CONSTANT;  
        strcpy(placement_radius.defined_by.constant.value,radius);  
        floor_radius.method_type = UF_MODL_PARM_CONSTANT;  
        strcpy(floor_radius.defined_by.constant.value,radius);  
    /* call the function to create the pocket 里海译:调用函数创建口袋。 */  
        UF_CALL(UF_MODL_create_general_pocket(&placement_faces,  
                                      &floor_faces,  
                                      &placement_outline,  
                                      &placement_radius,  
                                      &floor_radius,  
                                      &general_pocket_tag));  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是用于NX软件的二次开发，主要功能是创建一个带有倒角的方形口袋。以下是代码的主要功能介绍：
>
> 1. 创建一个方块体。
> 2. 创建4条直线，作为放置轮廓。
> 3. 确定方块体的放置面和楼面。
> 4. 设置放置轮廓、放置半径、楼面半径等参数。
> 5. 调用UF_MODL_create_general_pocket函数，创建带有倒角的方形口袋。
> 6. 在出错时，会输出错误信息。
> 7. 提供了ufusr和ufusr_ask_unload两个函数，供NX调用。
>
> 总的来说，这段代码实现了在NX中创建带有倒角的方形口袋的功能，并包含了错误处理和NX调用的接口。
>
