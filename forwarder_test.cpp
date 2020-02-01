#include <variant>
#include <string>
#include <vector>
#include <iostream>
#include <boost/hana.hpp>
#include <functional>
#include <variant>
#include <memory>
#include "gmock/gmock.h"

using JumpCallback = std::function<void(int)>;
using ::testing::_;
using ::testing::InvokeArgument;
using ::testing::InSequence;
using ::testing::SaveArg;

struct Input{
  virtual void listen_jump(JumpCallback) = 0;
};
struct Output{
  virtual void scream(int) = 0;
};

struct MockInput : public Input {
  MOCK_METHOD(void, listen_jump, (JumpCallback), (override));
};

struct MockOutput : public Output {
  MOCK_METHOD(void, scream, (int), (override));
};


class Translator {
  public:
    Translator(std::shared_ptr<Input> input, std::shared_ptr<Output> output) :
      output{output},
      input{input} {
        input->listen_jump([this](const int i) { this->output->scream(i); ;});
      }
    void poke_scream(int val) {
      output->scream(val);
    }
  private:
    std::shared_ptr<Output> output;
    std::shared_ptr<Input> input;

};

TEST(Test, TestEasy){
  auto input = std::make_shared<MockInput>();
  auto output = std::make_shared<MockOutput>();
  JumpCallback jump;
  {
    InSequence seq;
    EXPECT_CALL(*input, listen_jump)
                        .WillOnce(SaveArg<0>(&jump));
    EXPECT_CALL(*output, scream(1)).Times(1);
    EXPECT_CALL(*output, scream(2));
  }
  Translator translator {input, output};
  jump(1);
  jump(2);
}

int main(int argc, char** argv){
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
