
class CBenchMarkBase{
    public:
   
    virtual ~CBenchMarkBase() = 0;

    public:
    static CBenchMarkBase *CreateSelf();
    static void DestoryPtr(CBenchMarkBase *_ptr);
    //
    virtual bool RunBenchMarkTest(int argc, char *argv[]);
    //
    virtual bool SetupTest() = 0;
    //
    virtual bool SetupBenchMark() = 0;
    //
    virtual bool SetName(const char *_api_name) = 0;

    protected:
     CBenchMarkBase(){};
    protected:
    std::vector<std::string> m_log_str;
    std::string m_api_name;
};