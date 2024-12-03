
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <fmu4cpp/fmu_base.hpp>

class Model : public fmu4cpp::fmu_base {

public:
    Model(const std::string &instanceName, const std::filesystem::path &resources)
        : fmu_base(instanceName, resources) {

        register_variable(real("myReal", &real_)
                                  .setCausality(fmu4cpp::causality_t::OUTPUT));
        register_variable(integer("myInteger", &integer_)
                                  .setCausality(fmu4cpp::causality_t::OUTPUT));
        register_variable(boolean("myBoolean", &boolean_)
                                  .setCausality(fmu4cpp::causality_t::OUTPUT));
        register_variable(string("myString", &str_)
                                  .setCausality(fmu4cpp::causality_t::OUTPUT));

        Model::reset();
    }

    bool do_step(double currentTime, double dt) override {
        real_ = currentTime;
        ++integer_;
        boolean_ = !boolean_;
        str_ = std::to_string(integer_);
        return true;
    }

    void reset() override {
        boolean_ = false;
        integer_ = 0;
        real_ = 0;
        str_ = "0";
    }

private:
    bool boolean_;
    int integer_;
    double real_;
    std::string str_;
};

fmu4cpp::model_info fmu4cpp::get_model_info() {
    model_info m;
    m.modelIdentifier = FMU4CPP_MODEL_IDENTIFIER;
    return m;
}

std::unique_ptr<fmu4cpp::fmu_base> fmu4cpp::createInstance(const std::string &instanceName, const std::filesystem::path &fmuResourceLocation) {
    return std::make_unique<Model>(instanceName, fmuResourceLocation);
}

TEST_CASE("basic_test") {

    const auto instance = fmu4cpp::createInstance("", "");

    double t = 0;
    double dt = 0.1;

    auto real = instance->get_real_variable("myReal");
    REQUIRE(real);
    auto integer = instance->get_int_variable("myInteger");
    REQUIRE(integer);
    auto boolean = instance->get_bool_variable("myBoolean");
    REQUIRE(boolean);
    auto str = instance->get_string_variable("myString");
    REQUIRE(str);

    instance->setup_experiment(t, {}, {});
    instance->enter_initialisation_mode();
    instance->exit_initialisation_mode();

    unsigned int vr = boolean->value_reference();
    int testFail = false;
    REQUIRE_THROWS(instance->set_boolean(&vr, 1, &testFail));

    int i = 0;
    while (t < 10) {
        instance->do_step(t, dt);

        REQUIRE(real->get() == Catch::Approx(t));
        REQUIRE(boolean->get() == (i % 2 == 0));
        REQUIRE(integer->get() == ++i);
        REQUIRE(str->get() == std::to_string(i));

        t += dt;
    }

    instance->reset();

    REQUIRE(real->get() == 0);
    REQUIRE(boolean->get() == false);
    REQUIRE(integer->get() == 0);
    REQUIRE(str->get() == "0");

    instance->terminate();
}

TEST_CASE("wrong call order") {

    const auto instance = fmu4cpp::createInstance("", "");

    double t = 0;
    double dt = 0.1;

    auto real = instance->get_real_variable("myReal");
    REQUIRE(real);
    auto integer = instance->get_int_variable("myInteger");
    REQUIRE(integer);
    auto boolean = instance->get_bool_variable("myBoolean");
    REQUIRE(boolean);
    auto str = instance->get_string_variable("myString");
    REQUIRE(str);

    instance->setup_experiment(t, {}, {});
    instance->enter_initialisation_mode();
    instance->exit_initialisation_mode();

    int i = 0;
    while (t < 10) {
        instance->do_step(t, dt);

        REQUIRE(real->get() == Catch::Approx(t));
        REQUIRE(boolean->get() == (i % 2 == 0));
        REQUIRE(integer->get() == ++i);
        REQUIRE(str->get() == std::to_string(i));

        t += dt;
    }

    instance->reset();

    REQUIRE(real->get() == 0);
    REQUIRE(boolean->get() == false);
    REQUIRE(integer->get() == 0);
    REQUIRE(str->get() == "0");

    instance->terminate();
}
