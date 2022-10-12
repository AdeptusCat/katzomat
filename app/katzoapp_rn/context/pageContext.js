import React, {useState} from 'react';

export const PageContext = React.createContext();

export const PageContextProvider = () => {
  const [connected, setConnected] = useState(false);
  const [name, setName] = useState('katz');

  return (
    <PageContext.Provider value={[connected, setConnected, name, setName]} />
  );
};
