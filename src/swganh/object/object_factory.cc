#include "object_factory.h"


#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/sqlstring.h>
#include <glog/logging.h>

#include "anh/database/database_manager.h"
#include "swganh/object/object.h"
#include "swganh/object/exception.h"
#include "swganh/simulation/simulation_service.h"

using namespace sql;
using namespace std;
using namespace anh::database;
using namespace swganh::object;
using namespace swganh::simulation;

ObjectFactory::ObjectFactory(const shared_ptr<DatabaseManagerInterface>& db_manager,
                             SimulationService* simulation_service)
    : db_manager_(db_manager)
    , simulation_service_(simulation_service)
{
}

void ObjectFactory::CreateBaseObjectFromStorage(const shared_ptr<Object>& object, const shared_ptr<sql::ResultSet>& result)
{
    try {
        result->next();
        object->scene_id_ = result->getUInt("scene_id");
        object->position_ = glm::vec3(result->getDouble("x_position"),result->getDouble("y_position"), result->getDouble("z_position"));
        object->orientation_ = glm::quat(result->getDouble("x_orientation"),result->getDouble("y_orientation"), result->getDouble("z_orientation"), result->getDouble("w_orientation"));
        object->complexity_ = result->getDouble("complexity");
        object->stf_name_file_ = result->getString("stf_name_file");
        object->stf_name_string_ = result->getString("stf_name_string");
        string custom_string = result->getString("custom_name");
        object->custom_name_ = wstring(begin(custom_string), end(custom_string));
        object->volume_ = result->getUInt("volume");
        object->template_string_ = result->getString("iff_template");
        
    }
    catch(sql::SQLException &e)
    {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
    }
}

uint32_t ObjectFactory::LookupType(uint64_t object_id) const
{
    uint32_t type = 0;
    try {
        auto conn = db_manager_->getConnection("galaxy");
        auto statement = conn->prepareStatement("CALL sp_GetType(?);");
        statement->setUInt64(1, object_id);
        auto result = statement->executeQuery();        
        while (result->next())
        {
            type = result->getUInt("description");
        }
        return type;
    }
    catch(sql::SQLException &e)
    {
        DLOG(ERROR) << "SQLException at " << __FILE__ << " (" << __LINE__ << ": " << __FUNCTION__ << ")";
        DLOG(ERROR) << "MySQL Error: (" << e.getErrorCode() << ": " << e.getSQLState() << ") " << e.what();
        return type;
    }
}

void ObjectFactory::LoadContainedObjects(
    const shared_ptr<Object>& object,
    const shared_ptr<Statement>& statement)
{
    // Check for contained objects        
    if (statement->getMoreResults())
    {
        unique_ptr<ResultSet> result(statement->getResultSet());

        uint64_t contained_id;
        uint32_t contained_type;

        while (result->next())
        {
            contained_id = result->getUInt64("id");
            contained_type = result->getUInt("type_id");
            auto contained_object = simulation_service_->LoadObjectById(contained_id, contained_type);

            object->AddContainedObject(contained_object, Object::LINK);
        }
    }
}