//
// Created by 18163 on 2021/4/24.
//

#ifndef BPT_EXCEPTION_HPP
#define BPT_EXCEPTION_HPP
namespace RA {
    class exception {
    protected:
        const std::string variant = "";
        std::string detail = "";
    public:
        exception() {}
        exception(const exception &ec) : variant(ec.variant), detail(ec.detail) {}
        virtual std::string what() {
            return variant + " " + detail;
        }
    };

    class index_out_of_bound : public exception {
        /* __________________________ */
    };

    class file_fail : public exception {
        /* __________________________ */
    };
    class runtime_error : public exception {
        /* __________________________ */
    };
    class invalid_visit : public exception {
        /* __________________________ */
    };

    class container_is_empty : public exception {
        /* __________________________ */
    };

    class invalid_iterator : public exception {
        /* __________________________ */
    };
    class bad_tree_struct : public exception {
        /* __________________________ */
    };
    class bad_read_flow : public exception {
        /* __________________________ */
    };
    class bad_write_flow : public exception {
        /* __________________________ */
    };
    class unknown_field : public exception {
        /* __________________________ */
    };
}
#endif //BPT_EXCEPTION_HPP
