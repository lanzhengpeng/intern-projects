### 【1492】layer copy 图层复制

#### 代码

```cpp
    static void layer_copy(tag_t *objects, int n_objects, int to_layer)  
    {  
        int  
            copy = 2,  
            off = 2,  
            resp;  
        double  
            delta_zero[3] = { 0.0, 0.0, 0.0 },  
            mx[16];  
        tag_t  
            *copies;  
        copies = (tag_t *)UF_allocate_memory(n_objects * sizeof (tag_t), &resp);  
        UF_CALL(resp);  
        FTN(uf5943)(delta_zero, mx);  
        FTN(uf5947)(mx, objects, &n_objects, &copy, &to_layer, &off, copies, NULL,  
            &resp);  
        WRITENZ(resp);  
        UF_free(copies);  
    }

```

#### 代码解析

> 这段代码是NX Open API的二次开发代码，主要功能是实现NX中对象的层复制。具体来说，代码实现了以下功能：
>
> 1. 参数定义：定义了输入参数objects，表示要复制的对象数组，n_objects表示对象数组的长度，to_layer表示目标层。
> 2. 内存分配：通过调用UF_allocate_memory()函数为复制的对象分配内存空间，大小为n_objects个tag_t结构体的大小。
> 3. 变换矩阵计算：调用uf5943()函数计算一个单位变换矩阵，即物体位置不发生变化的变换矩阵。
> 4. 层复制：调用uf5947()函数进行层复制，将objects数组中的对象复制到to_layer层。其中，copy参数设置为2表示复制对象，off参数设置为2表示复制对象时保留原层信息，copies参数指向复制的对象数组。
> 5. 错误处理：通过WRITENZ()函数检查uf5947()的返回码，如果有错误发生，则输出错误信息。
> 6. 内存释放：调用UF_free()函数释放复制的对象数组内存空间。
>
> 总体来说，这段代码实现了NX中指定对象数组复制到目标层的功能。
>
