/**
 * @file Visualizer.cpp
 * @author Giulio Romualdi, Nicola Piga
 * @copyright Released under the terms of the MIT license
 * @date 2021
 */

#include <MeshcatCpp/QuitHandler.h>
#include <MeshcatCpp/Visualizer.h>

#include <mutex>
#include <pybind11/eigen.h>
#include <pybind11/embed.h>
#include <pybind11/stl.h>

using namespace MeshcatCpp;
namespace py = ::pybind11;

struct Visualizer::Impl
{
    std::unique_ptr<pybind11::scoped_interpreter> guard_;
    pybind11::object visualizer_;
    std::mutex mutex_;
};

Visualizer::~Visualizer() = default;

bool Visualizer::isValid() const
{
    return pimpl_ != nullptr;
}

void Visualizer::quit()
{
    if(!this->isValid())
        return;

    pimpl_.reset(nullptr);
}

Visualizer::Visualizer()
{
    pimpl_ = std::make_unique<Impl>();
    pimpl_->guard_ = std::make_unique<pybind11::scoped_interpreter>();
    pimpl_->visualizer_ = py::module::import("idyntree") //
                              .attr("visualize")
                              .attr("MeshcatVisualizer")();
}

void Visualizer::display(const Eigen::Ref<const Eigen::Vector3d>& position,
                         const Eigen::Ref<const Eigen::Matrix3d>& rotation,
                         const Eigen::Ref<const Eigen::VectorXd>& jointValue,
                         const std::string& modelName)
{
    if(!this->isValid())
        return;

    pimpl_->visualizer_.attr("display")(py::cast(position), //
                                        py::cast(rotation),
                                        py::cast(jointValue),
                                        modelName);
}

void Visualizer::open()
{
    if(!this->isValid())
        return;

    pimpl_->visualizer_.attr("open")();
}

void Visualizer::setModelFromFile(const std::string& modelPath,
                                  const std::vector<std::string>& jointList,
                                  const std::string& modelName)
{
    if(!this->isValid())
        return;

    pimpl_->visualizer_.attr("set_model_from_file")(modelPath, py::cast(jointList), modelName);
}

void Visualizer::loadModel(const std::vector<double>& color, const std::string& modelName)
{
    if(!this->isValid())
        return;

    pimpl_->visualizer_.attr("load_model")(modelName, py::cast(color));
}
