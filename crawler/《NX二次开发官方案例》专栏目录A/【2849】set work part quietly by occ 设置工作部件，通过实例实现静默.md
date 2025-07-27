### 【2849】set work part quietly by occ 设置工作部件，通过实例实现静默

#### 代码

```cpp
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void ensure_part_fully_loaded(tag_t part)  
    {  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            error_status;  
        if (!UF_CALL(UF_PART_ask_part_name(part, partname))  
            && ((UF_PART_is_loaded(partname)) != 1))  
        {  
            UF_CALL(UF_PART_open_quiet(partname, &part, &error_status));  
            if (error_status.n_parts > 0) report_load_status(&error_status);  
        }  
    }  
    static tag_t set_work_part_quietly_by_occ(tag_t occ)  
    {  
        tag_t  
            old_work_part = NULL_TAG,  
            owning_part,  
            proto = UF_ASSEM_ask_prototype_of_occ(occ);  
        if (!UF_ASSEM_is_occurrence(occ) ||  
            UF_OBJ_is_object_a_promotion(occ)) return NULL_TAG;  
        UF_CALL(UF_OBJ_ask_owning_part(proto, &owning_part));  
        ensure_part_fully_loaded(owning_part);  
        UF_CALL(UF_ASSEM_set_work_part_quietly(owning_part, &old_work_part));  
        return old_work_part;  
    }

```

#### 代码解析

> 这段代码是NX二次开发代码，其主要功能包括：
>
> 1. report_load_status函数：报告部件的加载状态。当有部件未成功加载时，它会打开一个窗口，并显示每个失败加载的部件名称及其错误信息。
> 2. ensure_part_fully_loaded函数：确保指定部件被完全加载。它会检查部件是否已加载，如果没有，则会静默加载该部件，并报告加载状态。
> 3. set_work_part_quietly_by_occ函数：根据给定的occurrence，设置当前工作部件。它会找到该occurrence的拥有部件，确保该部件被完全加载，然后静默设置该部件为当前工作部件。
>
> 整体来看，这段代码通过调用NX提供的API，实现了部件加载状态检查、加载失败报告、以及根据occurrence设置当前工作部件的功能。这些功能在NX二次开发中非常常见和实用。
>
