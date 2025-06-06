//
// FMU4cpp Hello World example
//

#include <fmu4cpp/fmu_base.hpp>


using namespace fmu4cpp;

class Model : public fmu_base {

public:
    explicit Model(const fmu_data &data) : fmu_base(data) {

        register_variable(integer(
                                  "integerIn", &integer_)
                                  .setCausality(causality_t::INPUT)
                                  .setVariability(variability_t::DISCRETE));
        register_variable(
                real(
                        "realIn", &real_)
                        .setCausality(causality_t::INPUT)
                        .setVariability(variability_t::DISCRETE));

        register_variable(boolean(
                                  "booleanIn", &boolean_)
                                  .setCausality(causality_t::INPUT)
                                  .setVariability(variability_t::DISCRETE));

        register_variable(string(
                                  "stringIn", &string_)
                                  .setCausality(causality_t::INPUT)
                                  .setVariability(variability_t::DISCRETE));

        register_variable(integer("integerOut", &integer_)
                                  .setCausality(causality_t::OUTPUT)
                                  .setVariability(variability_t::DISCRETE)
                                  .setInitial(initial_t::CALCULATED)
                                  .setDependencies({"integerIn"}));

        register_variable(real("realOut", &real_)
                                  .setCausality(causality_t::OUTPUT)
                                  .setVariability(variability_t::DISCRETE)
                                  .setInitial(initial_t::CALCULATED)
                                  .setDependencies({"realIn"}));

        register_variable(boolean("booleanOut", &boolean_)
                                  .setCausality(causality_t::OUTPUT)
                                  .setVariability(variability_t::DISCRETE)
                                  .setInitial(initial_t::CALCULATED)
                                  .setDependencies({"booleanIn"}));

        register_variable(string("stringOut", &string_)
                                  .setCausality(causality_t::OUTPUT)
                                  .setVariability(variability_t::DISCRETE)
                                  .setInitial(initial_t::CALCULATED)
                                  .setDependencies({"stringIn"}));

        Model::reset();
    }

    bool do_step(double dt) override {
        log(fmiOK, "hello@ " + std::to_string(currentTime()));
        return true;
    }

    void reset() override {
        integer_ = 0;
        real_ = 0;
        boolean_ = false;
        string_ = "empty";
    }

private:
    int integer_{};
    double real_{};
    bool boolean_{};
    std::string string_;
};

model_info fmu4cpp::get_model_info() {
    model_info info;
    info.modelName = "Identity";
    info.description = "A simple feed-trough model";
    info.modelIdentifier = FMU4CPP_MODEL_IDENTIFIER;
    return info;
}

FMU4CPP_INSTANTIATE(Model);// Entry point for FMI instantiate function.
