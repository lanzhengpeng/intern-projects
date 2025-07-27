### 【2848】set work part quietly by occ 根据实例静默设置工作部件

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

> 这是段NX Open C++的二次开发代码，主要用于处理NX部件的加载状态。
>
> 1. report_load_status函数：该函数用于报告NX部件的加载状态。它接收一个UF_PART_load_status_p_t类型的参数，其中包含加载失败部件的文件名和状态码。函数会遍历所有加载失败的部件，获取它们的文件名和失败信息，并在NX的Listing窗口中输出。
> 2. ensure_part_fully_loaded函数：该函数确保指定的NX部件被完全加载。它接收部件的tag参数，如果部件未加载，则会调用UF_PART_open_quiet函数尝试加载，并报告加载状态。
> 3. set_work_part_quietly_by_occ函数：该函数用于设置NX的工作部件。它接收一个NX出现的tag参数，获取出现对应的原型部件，并确保原型部件被完全加载。然后调用UF_ASSEM_set_work_part_quietly设置原型部件为工作部件，并返回旧的工作部件。
>
> 总体来说，这段代码通过获取NX部件的加载状态，确保需要操作的部件被完全加载，并设置NX的工作部件，以便进行后续的NX操作。
>
