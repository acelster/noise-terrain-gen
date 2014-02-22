#include <nmlib/model.hpp>

#include <gtest/gtest.h>

#include <fstream>

namespace {

TEST(ModelTest, SignalType){
    auto signalType = nm::SignalType{4};
    EXPECT_EQ(4, signalType.dimensionality);
}

TEST(ModelTest, BuiltinModuleType){
    nm::ModuleType moduleType{"test", "testdescription"};
    EXPECT_EQ("test", moduleType.getName());
    EXPECT_EQ("testdescription", moduleType.getDescription());

    //inputs
    moduleType.addInput("testinput", nm::SignalType{2});
    auto input = moduleType.getInput("testinput");
    ASSERT_NE(nullptr, input);
    EXPECT_EQ("testinput", input->getName());
    EXPECT_EQ(2, input->getSignalType().dimensionality);

    //outputs
    moduleType.addOutput("testoutput", nm::SignalType{3});
    auto output = moduleType.getOutput("testoutput");
    ASSERT_NE(nullptr, output);
    EXPECT_EQ("testoutput", output->getName());
    EXPECT_EQ(3, output->getSignalType().dimensionality);
}

TEST(ModelTest, PrimitiveModuleTypeBasic){
    //TODO why doesn't it work with a simple stack allocation?
    std::unique_ptr<nm::ModuleType> moduleType{new nm::ModuleType("test", nm::ModuleType::Category::Primitive, "testdescription")};
    EXPECT_EQ("test", moduleType->getName());
    EXPECT_EQ("testdescription", moduleType->getDescription());
    EXPECT_FALSE(moduleType->isComposite());
    EXPECT_TRUE(moduleType->isPrimitive());
}

TEST(ModelTest, CompositeModuleTypeBasic){
    //TODO why doesn't it work with a simple stack allocation?
    std::unique_ptr<nm::ModuleType> moduleType{new nm::ModuleType("test", "testdescription")};
    EXPECT_EQ("test", moduleType->getName());
    EXPECT_EQ("testdescription", moduleType->getDescription());
    EXPECT_TRUE(moduleType->isComposite());
    EXPECT_FALSE(moduleType->isPrimitive());
}

TEST(ModelTest, CompositeModuleTypeInputAdding){
    //TODO why doesn't it work with a simple stack allocation?
    std::unique_ptr<nm::ModuleType> moduleType{new nm::ModuleType("test")};
    ASSERT_TRUE(moduleType->isComposite());
    moduleType->addInput("in1", nm::SignalType{1});
    auto moduleInput = moduleType->getInput("in1");
    EXPECT_EQ("in1", moduleInput->getName());
    auto graph = moduleType->getGraph();
    ASSERT_NE(nullptr, graph);
    auto inputsModule = graph->getModule("inputs");
    ASSERT_NE(nullptr, inputsModule);
    EXPECT_NE(nullptr, inputsModule->getInput("in1"));
}

TEST(ModelTest, TopologicalTraversal){
    //create a simple test module type
    std::unique_ptr<nm::ModuleType> moduleType{new nm::ModuleType("test", "testdescription")};
    moduleType->addInput("in1", nm::SignalType{1});
    moduleType->addInput("in2", nm::SignalType{1});
    moduleType->addOutput("result", nm::SignalType{1});

    //create some modules
    nm::Module a(*moduleType, "a");
    nm::Module b(*moduleType, "b");
    nm::Module c(*moduleType, "c");
    nm::Module d(*moduleType, "d");

    //connect them together. a is connected to b which is connected to both c and d
    a.getInput("in1")->link(*b.getOutput("result"));
    b.getInput("in1")->link(*c.getOutput("result"));
    b.getInput("in2")->link(*d.getOutput("result"));

    std::vector<std::string> names;

    auto appendName = [&](nm::Module& module){
        names.push_back(module.getName());
    };

    nm::Module::topologicallyTraverseDependencies({a.getOutput("result")}, appendName);
    EXPECT_EQ("a", names.back()); //we expect a to be last because it depends on everything
    EXPECT_EQ("b", names[2]); //b depends on everything except a, so we expect it to be second last
    EXPECT_EQ(4, names.size()); //just check that all nodes were visited
}

}
