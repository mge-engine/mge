#include "mge/graphics/pipeline.hpp"
#include "opengl.hpp"

namespace opengl {
    class pipeline : public mge::pipeline
    {
    public:
        pipeline(mge::render_context& context);
        virtual ~pipeline();
    protected:
        void on_link();
        void on_set_shader(const mge::shader_ref& shader);
    private:
        void dump_info_log();
        GLuint m_program;
    };
}