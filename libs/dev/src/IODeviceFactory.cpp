#include "protodb/factories/IODeviceFactory.h"

#include <protodb/factories/GlobalFactoryStorage.h>
#include <protodb/creators/IODeviceCreatorInterface.h>

using namespace protodb;

IODeviceFactory::IODeviceFactory(QObject* parent)
    : FactoryAbstract(parent)
{

}

bool IODeviceFactory::addCreator(const QSharedPointer<IODeviceCreator>& creator)
{
    if (!creator || containsCreator(creator->cid())) {
        return false;
    }

    m_creators[ creator->cid() ] = creator;

    return true;
}

QList<QSharedPointer<IODeviceCreator> > IODeviceFactory::getAllCreators() const
{
    QList<QSharedPointer<IODeviceCreator> > ret;

    for (auto& it: m_creators) {
        auto ptr = qSharedPointerCast<IODeviceCreator>(it);
        if (ptr) {
            ret.append(ptr);
        }
    }

    return ret;
}

QSharedPointer<IODeviceCreator> IODeviceFactory::getCreator(const QString& cid) const
{
    if (m_creators.contains(cid)) {
        return qSharedPointerCast<IODeviceCreator>(m_creators[cid]);
    }

    return nullptr;
}

QSharedPointer<IODeviceCreator> IODeviceFactory::operator[](const QString& cid) const
{
    if (m_creators.contains(cid)) {
        return qSharedPointerCast<IODeviceCreator>(m_creators[cid]);
    }

    return nullptr;
}

const QString& IODeviceFactory::getDefaultCreator() const
{
    return m_default_cid;
}

void IODeviceFactory::setDefaultCreator(const QString& cid)
{
    if (m_creators.contains(cid))
        m_default_cid = cid;
}

QIODevice* IODeviceFactory::createIODevice(QString& desc) const
{
    return createIODevice(m_default_cid, desc);
}

QIODevice* IODeviceFactory::createIODevice(const nlohmann::json& json, QString& desc) const
{
    return createIODevice(m_default_cid, json, desc);
}

QIODevice* IODeviceFactory::createIODevice(const QString& cid, QString& desc) const
{
    if( m_creators.contains(cid) )
    {
        auto creator = qSharedPointerCast< IODeviceCreator >( m_creators[ cid ] );
        if( ! creator )
            return nullptr;

        return creator->create(desc);
    }

    return nullptr;
}

QIODevice* IODeviceFactory::createIODevice(const QString& cid, const nlohmann::json& json, QString& desc) const
{
    if( m_creators.contains(cid) )
    {
        auto creator = qSharedPointerCast< IODeviceCreator >( m_creators[ cid ] );
        if( ! creator )
            return nullptr;

        return creator->create(json, desc);
    }

    return nullptr;
}

QPointer<IODeviceFactory> IODeviceFactory::globalInstance()
{
    return qobject_cast< IODeviceFactory * >( GlobalFactoryStorage::getFactory( fid() ) );
}

QString IODeviceFactory::fid()
{
    return QString("IODeviceFactory");
}

