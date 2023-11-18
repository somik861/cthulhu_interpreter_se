#pragma once

#include "program/istack.h"
#include "program/istack_item.h"

#include "utils/imap.h"
#include "utils/iset.h"

namespace cthu::program {
class IDict : public virtual IStackItem {
  public:
    using mapping_type = utils::IMap<std::size_t, std::unique_ptr<IStack>>;

    virtual ~IDict() = default;

    /**
     * @brief Get stack it given idx
     *
     * @param[in] idx   index
     *
     * @return          pointer to stack or nullptr if not exists
     */
    virtual IStack* at(std::size_t idx) = 0;
    /**
     * @brief Get stack it given idx
     *
     * @param[in] idx   index
     *
     * @return          pointer to stack or nullptr if not exists
     */
    virtual const IStack* at(std::size_t idx) const = 0;

    /**
     * @brief Set stack on given index
     *
     * @param[in] idx        index
     * @param[in] stack      stack
     */
    virtual void set(std::size_t idx, std::unique_ptr<IStack> stack) = 0;
    /**
     * @brief Get set of available keys
     *
     * @return       available keys
     */
    virtual std::unique_ptr<utils::ISet<std::size_t>> getKeys() const = 0;

    /**
     * @brief Get copy of a dictionary
     *
     * @return        dictionary copy
     */
    virtual std::unique_ptr<IDict> clone() const = 0;
    /**
     * @brief Get empty dictionary of the same type
     *
     * @return       empty dictionary
     */
    virtual std::unique_ptr<IDict> getEmpty() const = 0;

    static std::unique_ptr<IDict> createDict(std::unique_ptr<mapping_type> backend = mapping_type::createStdMap());
};
} // namespace cthu::program
