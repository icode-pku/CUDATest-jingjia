namespace TestCUDA
{
#define BEGIN_EXCEPTION \
    try                 \
    {
#define END_EXCEPTION(T)               \
    }                                         \
    catch (T & err)                           \
    {                                         \
        Log(err.what());                      \
        AddError(err.what());          \
    }                                         \
    catch (...)                               \
    {                                         \
        AddError("exception error!!"); \
    }

#define END_SUCESS(flags) AddError("SUCESS!"); 
#define CUDA_STREAM_GROUP "cuda_stream_group"
#define CUDA_EVENT_GROUP "cuda_event_group"
};