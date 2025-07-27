### 【0142】add elbows to selected routing control points 向选定的路由控制点添加弯头

#### 代码

```cpp
    /*HEAD ADD_ELBOWS_TO_SELECTED_ROUTING_CONTROL_POINTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_route.h>  
    #include <uf_object_types.h>  
    #include <uf_dirpath.h>  
    #include <uf_assem.h>  
    #include <uf_attr.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，我翻译如下：

UF_print_syslog是V18版本新增的函数。 */  
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
    /* 里海 */  
    static int mask_for_rcps(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_route_control_point_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_rcps(char *prompt, tag_t **rcps)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select Routing Control Points",  
            prompt, UF_UI_SEL_SCOPE_WORK_PART, mask_for_rcps, NULL, &resp,  
            &cnt, rcps));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*rcps)[ii], FALSE));  
        return cnt;  
    }  
    static void do_it(void)  
    {  
        int  
            inx = 0,  
            n_std_part_charx,  
            n_std_part_matches,  
            num_places,  
            num_rcps;  
        tag_t  
            dir_path,  
            fit_part,  
            inst_id,  
            occ,  
            *rcps,  
            work_part,  
            work_part_comp,  
            work_part_occ;  
        double  
            csys[6] = { 1.0, 0.0,  0.0, 0.0, 1.0, 0.0 },  
            origin[3] = { 0.0, 0.0, 0.0 },  
            new_origin[3],  
            new_csys[6];  
        char  
            *app_view_name = NULL,  
            *app_view_symb = "UGII_BASE_APP_VIEW",  
            full_name[MAX_FSPEC_SIZE],  
            *part_search_symb = "UGII_ROUTING_PART_PATH",  
            *std_part_material = "STEEL",  
            *std_part_rating   = "40";  
        UF_PART_load_status_t  
            error_code;  
        UF_ROUTE_charx_t  
            std_part_charx[4];  
        UF_ROUTE_part_lib_part_p_t  
            std_part_matches;  
        UF_ROUTE_application_view_p_t  
            app_view,  
            old_app_view;  
        UF_ROUTE_place_solution_p_t  
            *places;  
     /* Load and set the application view *** Routing -> Base 译:翻译为：加载并设置应用视图 Routing -> Base */  
        old_app_view = UF_ROUTE_ask_current_app_view( );  
        UF_CALL(UF_translate_variable( app_view_symb, &app_view_name ));  
        UF_CALL(UF_ROUTE_load_app_view( app_view_name, &app_view ));  
        UF_CALL(UF_ROUTE_set_current_app_view( app_view ));  
     /* Set the search path for locating Routing (Base) parts / stock 译:设置搜索路径以定位路由（基础）零件/库存 */  
        UF_CALL(UF_DIRPATH_create_from_env ( part_search_symb, &dir_path ));  
        UF_CALL(UF_ROUTE_set_part_search_path ( dir_path ));  
        num_rcps = select_rcps("Add Elbows", &rcps);  
     /*  Place standard part elbows at the RCPs 译:请将标准弯头安装在RCPs上。 */  
        for (inx = 0; inx < num_rcps; inx++)  
        {  
        /*  Set up the characteristics for the Standard part we wish to place 译:设置我们要放置的标准部件的特性。 */  
            std_part_charx[0].type = UF_EPLIB_CHARX_TYPE_REAL;  
            strcpy (std_part_charx[0].title, "DIAMETER");  
            std_part_charx[0].value.r_value = 0.75;  
            std_part_charx[1].type = UF_EPLIB_CHARX_TYPE_REAL;  
            strcpy (std_part_charx[1].title, "ELBOW_ANG");  
            std_part_charx[1].value.r_value = 90.0;  
            std_part_charx[2].type = UF_EPLIB_CHARX_TYPE_STR;  
            strcpy (std_part_charx[2].title, "FITTING_MATERIAL");  
            std_part_charx[2].value.s_value = std_part_material;  
            std_part_charx[3].type = UF_EPLIB_CHARX_TYPE_STR;  
            strcpy (std_part_charx[3].title, "RATING");  
            std_part_charx[3].value.s_value = std_part_rating;  
            n_std_part_charx = sizeof(std_part_charx) / sizeof(std_part_charx[0]);  
            UF_CALL(UF_ROUTE_match_charx_in_plib( "ROUND_ELBOW", n_std_part_charx,  
                                          std_part_charx, &n_std_part_matches,  
                                          &std_part_matches ));  
            if (n_std_part_matches == 0) continue;  
            UF_CALL(UF_ROUTE_load_part_by_charx( std_part_matches[0].num_charx,  
                std_part_matches[0].charx, &fit_part ));  
         /* Add the elbow as a component of the work part 译:请将弯头作为工件的一部分添加。 */  
            UF_PART_ask_part_name( fit_part, full_name );  
            work_part = UF_ASSEM_ask_work_part( );  
            UF_CALL(UF_ASSEM_add_part_to_assembly( work_part, full_name, NULL,  
                                           NULL, origin, csys, -1,  
                                           &inst_id, &error_code ));  
            /*  
                For the following Routing operations of setting the characteristics  
                of the fitting part and for placing it within the Routing, we  
                use the part occurrence of the fitting in the part occurrence tree  
                that is "rooted" at the work part. This is because the Routing  
                characteristics and placement functions expect this particular  
                part occurrence.  
            */  
            work_part_comp = UF_ASSEM_ask_parent_of_instance( inst_id );  
            work_part_occ = UF_ASSEM_ask_root_part_occ( work_part_comp );  
            occ = UF_ASSEM_ask_part_occ_of_inst( work_part_occ, inst_id ) ;  
            /*  
                We now have the part occurrence of the fitting in the work part's  
                part occurrence tree. So we can attach the Routing characteristics  
                to this part occurrence and place this occurrence within the  
                Routing.  
            */  
            UF_CALL(UF_ROUTE_set_characteristics( occ,  
                std_part_matches[0].num_charx, std_part_matches[0].charx ));  
         /* Place the elbow at the RCPs 译:将肘部放在RCPs上 */  
            num_places = 0; places = NULL;  
            UF_CALL(UF_ROUTE_solve_places( rcps[inx], occ, &num_places, &places ));  
            UF_ROUTE_ask_places_transform( places[0], new_origin, new_csys );  
            UF_CALL(UF_ASSEM_reposition_instance( inst_id, new_origin, new_csys ));  
            UF_CALL(UF_ROUTE_free_places( num_places, places ));  
            UF_CALL(UF_ROUTE_set_part_in_stock( occ ));  
        }  
        if (num_rcps > 0) UF_free(rcps);  
        UF_CALL(UF_ROUTE_unload_app_view( app_view ));  
        UF_CALL(UF_ROUTE_set_current_app_view( old_app_view ));  
    }  
    /* 里海 */  
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

> 这段代码是一个NX的二次开发程序，主要用于在NX中实现向选定的路由控制点添加弯头的功能。主要功能包括：
>
> 1. 初始化和错误报告：初始化NX环境，并设置了错误报告函数。
> 2. 选择路由控制点：提供了函数用于选择路由控制点，并进行了错误处理。
> 3. 添加弯头：实现了加载弯头标准件、匹配弯头特征、添加弯头到装配、设置弯头特征、在路由控制点放置弯头，并进行了错误处理。
> 4. 清理和结束：释放资源、恢复视图，并结束NX环境。
>
> 总体来说，该代码实现了在NX中向选定的路由控制点添加弯头的完整流程，具有一定的实用价值。
>
